# Quant Market Simulator

An event-driven quantitative market simulation framework built in C++ for developing and testing algorithmic trading strategies in realistic market environments.

## Overview

This project aims to simulate a realistic electronic market microstructure environment for:
- algorithmic trading research
- agent-based strategy simulation
- market-making experimentation
- quantitative trading competitions
- execution and orderbook modelling

The long-term goal is to create a modular simulation engine where multiple autonomous trading agents interact through a simulated limit order book driven by a stochastic mid-price process and news events.

---

# Current Features

## Mid-Price Simulation Engine

The current system models a stochastic mid-price process using:

- Geometric Brownian Motion (GBM)
- Event-driven jump diffusion
- Mean-reverting stochastic volatility
- Temporary drift and volatility regime shifts
- Deterministic and random news events

The model currently supports:
- quarterly earnings jumps
- macroeconomic shocks
- firm-specific rumors
- volatility amplification during news regimes

---

# Current Price Model

The simulated mid-price price follows a jump-diffusion stochastic process:

```math
S_{t+1} = S_t \cdot \exp\left(
J_t +
(\mu_t - \frac{1}{2}\sigma_t^2)dt +
\sigma_t\sqrt{dt}\epsilon_t
\right)
```

Where:

- $S_t$ = asset mid-price at time $t$
- $J_t$ = news-driven jump component
- $\mu_t$ = effective drift term
- $\sigma_t$ = effective volatility
- $\epsilon_t \sim \mathcal{N}(0,1)$ = standard Gaussian noise

Additional features:
- stochastic volatility mean reversion
- volatility regime shifts
- event persistence
- jump clustering through overlapping news events

## Volatility Dynamics

Baseline volatility evolves through a mean-reverting stochastic process:

```math
\sigma_{t+1}^{base}
=
\sigma_t
+
\theta(\sigma_0 - \sigma_t)dt
+
\eta \epsilon_t^{(vol)}
```

Effective volatility is then adjusted by active news regimes:

```math
\sigma_t^{eff}
=
\sigma_t^{base}(1 + 3V_t)
```

Where:

- $\theta$ = mean reversion speed
- $\sigma_0$ = long-run volatility level
- $\eta$ = volatility-of-volatility parameter
- $V_t$ = cumulative volatility impact from active news events

---

## News Event Effects

Each news event may temporarily alter:

- price jumps
- drift
- volatility
- market regime persistence

Examples include:
- quarterly earnings releases
- macroeconomic shocks
- firm-specific rumors

News effects can overlap, creating clustered volatility regimes and jump cascades.

---

# Architecture

## Current Components

### `PriceModel`
Handles:
- GBM simulation
- jump diffusion
- stochastic volatility
- event impacts
- simulation stepping

### `NewsGenerator`
Creates:
- scheduled earnings events
- random macro events
- rumor/news shocks

### `RandomEngine`
Centralized seeded RNG system for deterministic reproducibility.

### `NewsEvent`
Represents market-moving events with:
- jump magnitude
- volatility impact
- drift impact
- duration
- headline metadata

---

# Example Output

The engine currently exports:
- simulated price paths
- volatility values
- drift terms
- jump magnitudes
- news events

Output is written to CSV for:
- visualization
- statistical analysis
- strategy testing

---

# Planned Features

## Limit Order Book

Implementation of a realistic order book with:
- bid/ask queues
- order matching
- cancellations
- market orders
- queue priority
- spread dynamics

The current mid-price process will later act as a latent fair-value reference around which agents quote prices.

---

## Trading Agents

Planned agent types include:

### Market Makers
- quote around fair value
- inventory management
- spread optimization

### Trend Followers
- momentum-based execution
- breakout trading

### Mean Reversion Agents
- statistical arbitrage
- microstructure reversion
- reduce impact of large moves

### News-Based Agents
- react to event releases
- volatility exploitation

### Reinforcement Learning Agents
- adaptive execution strategies
- learned quoting behaviour

### Random Traders
- take random directional trades at random sizes

---

# Long-Term Goals

- realistic exchange simulation
- high-frequency trading environment
- execution quality analysis
- agent interaction studies
- latency modelling
- transaction cost modelling
- slippage simulation
- synthetic market generation for competitions

---

# Tech Stack

- C++17
- STL
- Object-Oriented Design
- Event-Driven Architecture

Planned:
- Python analytics layer
- visualization tools
- parallelized simulations
- real market data calibration

---

# Example Build

```bash
g++ src/*.cpp -I include -o simulation.exe -std=c++17