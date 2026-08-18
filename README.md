# Quant Market Simulator

An event-driven market simulation engine written in C++17, built to research and test algorithmic trading strategies against a synthetic but statistically realistic market — a stochastic mid-price process, scheduled and random news shocks, and a small population of autonomous trading agents.

The long-term goal is a full simulated exchange: a stochastic fair-value process feeding a real limit order book, with multiple agent strategies competing to fill orders, get marked to market, and be scored on standard performance metrics.

---

## Status at a glance

| Component | State |
|---|---|
| Stochastic price & news engine | **Built** — jump-diffusion GBM with mean-reverting volatility and news events |
| Trading strategies (Random, Momentum, Avellaneda–Stoikov market maker) | **Built** — each generates orders from price history |
| Trader wrapper (cash, inventory, PnL, fill history) | **Built** |
| Performance metrics (Sharpe, drawdown, inventory RMSE, fill rate) | **Built**, not yet wired into the demo executables below |
| Probabilistic fill model & spread model | **Built** — lightweight, distance/size-based fill probability; not a real matching engine |
| Limit order book & matching engine | **Not started** — header/source files exist but are currently empty; this is the next major milestone |
| Visualization | **Built** — CSV export + a small `PricePlot.py` matplotlib script, plus the interactive web app below |
| Interactive web app (tune agent counts/parameters, see PnL live) | **Built** — see [`../WebApp`](../WebApp) |

The strategies currently run in isolated demo loops that read simulated price history and print the orders they *would* place — they aren't yet routed through a real order book, so nothing actually fills or clears against another agent. That wiring is the current focus.

---

## Web app — Strategy Lab

[`../WebApp`](../WebApp) is a browser-based companion to this engine: pick how many
agents run each strategy, tune their parameters (momentum factor, lookback window,
risk aversion, order arrival intensity, and more), and watch each strategy's PnL
update live against a shared simulated market. It's a TypeScript port of this
engine's math (price model, strategies, fill model, metrics) running entirely
client-side — see that folder's README for exactly what was ported as-is and what
was deliberately changed. Two of those changes are worth pulling back into this
C++ engine if you want the two to stay behaviourally identical:

- `PriceModel`'s `volatility` constructor argument is currently unused here —
  `sigma_baseline`/`sigma0` are hardcoded to `0.012` regardless of what's passed
  in, so that parameter has no effect.
- `Trader::onFill` never calls `strategy->onFill()`, so
  `ASMarketMakingStrategy`'s own inventory tracking (and therefore its
  reservation-price skew) never actually updates in the current demos.

---

## Architecture

```
CppEngine/
├── include/                 # public headers
├── src/
│   ├── engine/               # PriceModel, NewsGenerator, RandomEngine, SpreadModel, FillModel
│   ├── agents/                # Trader, AgentMetrics
│   ├── strategies/            # RandomStrategy, MomentumStrategy, ASMarketMakingStrategy
│   ├── orderbook/             # OrderBook, MatchingEngine — stubs, not yet implemented
│   └── tests/                 # standalone demo executables for each piece
├── CMakeLists.txt
└── PricePlot.py               # plots price_simulation.csv
```

### Engine (`src/engine/`)

- **`PriceModel`** — steps the mid-price forward each tick using a jump-diffusion GBM process with mean-reverting stochastic volatility (see [Price Model](#price-model) below). Tracks the last drift, volatility, and jump applied, and exposes the active news events for the day.
- **`NewsGenerator`** — produces a fixed quarterly earnings calendar plus randomly sampled macro shocks and firm-specific rumors, each with its own jump size, drift impact, volatility impact, and duration.
- **`RandomEngine`** — a single seeded RNG wrapper (uniform, normal, randint) shared across the engine so simulations are reproducible.
- **`SpreadModel`** — a minimal linear spread model (`base + k · volatility`).
- **`FillModel`** — a probabilistic fill model: fill probability decays with distance from mid and with order size. This is what currently stands in for a real order book when a strategy's order needs to be evaluated for a fill.

### Agents & strategies (`src/agents/`, `src/strategies/`)

All strategies implement a common `Strategy` interface (`generateOrders(history)`, `getTypeTag()`, optional `onFill(qty)`):

- **`RandomStrategy`** — random direction, random size, small random offset from the touch. Exists mainly to give the other strategies realistic fill probabilities to trade against.
- **`MomentumStrategy`** — compares price now to price `lookbackPeriod` steps ago; if the move exceeds a threshold, submits a single order in the direction of momentum, sized by a `momentumFactor`.
- **`ASMarketMakingStrategy`** — a two-sided quoting strategy based on the Avellaneda–Stoikov market-making model: computes a reservation price that skews away from the mid based on current inventory, risk aversion (`gamma`), and time remaining in the horizon, then quotes a symmetric half-spread around it.

`Trader` wraps a strategy with capital: it owns cash, inventory, and fill/PnL history, and exposes realised/unrealised/total PnL. `AgentMetrics::computeMetrics()` turns a trader's PnL and inventory history into Sharpe ratio, max drawdown, inventory RMSE, and fill rate — implemented and unit-testable today, just not yet called from the demo executables.

### Order book (`src/orderbook/`) — planned

`OrderBook`, `MatchingEngine`, `Fill`, and `Trade` exist as empty files. The intended design (from working notes):

- Bids sorted descending by price, asks sorted ascending.
- An incoming order is either **passive** (worse than the best opposite quote, rests in the book) or **aggressive** (crosses the spread, generates a fill immediately).
- Per-tick flow: `PriceModel` steps → agents call `generateOrders()` → orders arrive at `OrderBook` → `MatchingEngine` matches and produces `Fill`s → fills are pushed back to agents via `onFill()` → PnL, spread, and inventory update.
- `PriceModel`'s output is meant to become a latent **fair value** rather than the tradeable price itself — the order book's own supply/demand will let the traded price occasionally track, diverge from, and overshoot fair value, which is the realistic behaviour the probabilistic `FillModel` can't produce on its own.

---

## Price model

The mid-price follows a jump-diffusion process:

```math
S_{t+1} = S_t \cdot \exp\left(
J_t +
(\mu_t - \tfrac{1}{2}\sigma_t^2)dt +
\sigma_t\sqrt{dt}\,\epsilon_t
\right)
```

- $S_t$ — mid-price at time $t$
- $J_t$ — sum of jump contributions from any news events active on day $t$
- $\mu_t$ — effective drift (base drift + active news drift impact)
- $\sigma_t$ — effective volatility
- $\epsilon_t \sim \mathcal{N}(0,1)$

Baseline volatility follows a mean-reverting (Ornstein–Uhlenbeck-style) process:

```math
\sigma_{t+1}^{base} = \sigma_t^{base} + \theta(\sigma_0 - \sigma_t^{base})\,dt + \eta\,\epsilon_t^{(vol)}
```

and is scaled up by any active news regime:

```math
\sigma_t^{eff} = \sigma_t^{base}\,(1 + 3V_t)
```

where $\theta$ is the mean-reversion speed, $\sigma_0$ the long-run volatility level, $\eta$ the vol-of-vol, and $V_t$ the cumulative volatility impact of active news events. Effective volatility is capped to prevent blow-ups.

News events come in three flavours, each with its own jump size, drift impact, volatility impact, and duration, and can overlap to produce clustered volatility and jump cascades:

- **Scheduled quarterly earnings** — fixed calendar days, moderate jump/drift/vol impact.
- **Macro shocks** — randomly sampled, larger volatility impact, 3-day duration.
- **Firm-specific rumors** — randomly sampled, smaller impact, 2-day duration.

---

## Building and running

Requires CMake ≥ 3.15 and a C++17 compiler.

```bash
cmake -B build -S .
cmake --build build
```

This produces four separate executables, one per demo (there's no single "run everything" binary yet — `main.cpp` at the project root is currently a placeholder):

| Target | What it does |
|---|---|
| `price_sim` | Runs the price/news engine standalone over a 2,016-day horizon and writes `price_simulation.csv` |
| `as_test` | Runs the Avellaneda–Stoikov market maker against a simulated price path and prints its quotes |
| `momentum_trader_test` | Runs the momentum strategy and prints signals/orders |
| `random_trader_test` | Runs the random strategy and prints orders |

```bash
./build/price_sim
python PricePlot.py   # plots mid/bid/ask from price_simulation.csv (needs pandas, matplotlib)
```

---

## Roadmap

### Near-term
- Implement `OrderBook` and `MatchingEngine` so orders actually match and fills flow back through `Trader::onFill()`, replacing the probabilistic `FillModel` shortcut for strategies that need real price impact.
- Wire `AgentMetrics` into the demo executables so each run reports Sharpe, drawdown, inventory RMSE, and fill rate rather than just printing raw orders.
- ~~A lightweight dashboard/monitoring view~~ — done, see [`../WebApp`](../WebApp).
- Let the order book's own trading activity feed back into price, so the current mid-price process becomes a latent fair value rather than the traded price itself.
- Once a real order book exists, extend the web app to route agents through it instead of the probabilistic fill model, so agents trade against each other rather than an independent shared path.

### Mid-term — additional agent archetypes
Ideas captured from breaking down real trading-competition strategy types, to implement as new `Strategy` subclasses:
- **Options/derivatives** — delta-neutral strategies, a lightweight Black–Scholes pricer, implied-vs-realised vol signals.
- **Statistical arbitrage** — pairs trading and rolling z-score signals, orderbook-imbalance short-term signals.
- **Event-driven** — decision-tree style reactions to news archetypes (earnings beat/miss, M&A, rate changes), building on the existing `NewsEvent` system.
- Mechanically enforced risk limits (hard position limits, stop-losses) across all agents.

### Long-term
- Reinforcement learning agents with adaptive execution/quoting behaviour.
- Latency modelling, transaction costs, and slippage simulation.
- Parallelized simulations and calibration against real market data.
- A Python analytics layer beyond the current single plotting script.

---

## Tech stack

- **C++17**, STL, CMake
- **Python** (pandas, matplotlib) for visualization of simulation output

---

## Repo layout

```
CppEngine/
├── CMakeLists.txt
├── main.cpp                # placeholder entry point
├── PricePlot.py
├── include/                 # headers for engine, agents, strategies
└── src/
    ├── engine/
    │   ├── PriceModel.cpp / .h
    │   ├── NewsGenerator.cpp / .h
    │   ├── RandomEngine.cpp / .h
    │   ├── SpreadModel.cpp / .h
    │   └── FillModel.cpp / .h
    ├── agents/
    │   ├── Trader.cpp / .h
    │   └── AgentMetrics.cpp / .h
    ├── strategies/
    │   ├── RandomStrategy.cpp / .h
    │   ├── MomentumStrategy.cpp / .h
    │   └── ASMarketMakingStrategy.cpp / .h
    ├── orderbook/            # OrderBook, MatchingEngine — planned
    └── tests/                # price_sim, as_test, momentum_trader_test, random_trader_test
```
