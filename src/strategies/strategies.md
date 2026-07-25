## Trading Competition Focus
# Breaking down types of strategies employed in a number of tradiong competitions andlooking at how they can be implemented into my engine as agents

## 1. Market Making / Liquidity Provision

    Focus on spread capture rather than directionality
    Dynamic spread widening with something like Avelaned-Stoikov 
    (inventory dependent)
    Fast requoting after fills to avoid adverse selection and being picked-off

## 2. Options and Derivatives Trading

    Delta-neutral strategies, implied vs. realized vol
    Greeks dashboards on the fly (lightweight BS pricer)
    Volatility arb between correlated / co-integrated instruments

## 3. Statistical / Algorithmic

    Pairs and stat arb using rolling z
    Momentum and mean reversion using signal blending, kept simple to avoid overfit
    Orderbook imbalance and ST signal

## 4. Event driven and news

    Fastest and most accurate interpretation - even assigning one to buil dstrat / disc. trade it
    Pre-built decision trees for common news archetypes (earnings beat/miss, M&A, rate change)


# Things to be mindful of
## Risk and execution discipline
    Hard position limit and stop-loss rules, mechanically enforced
    Team role specialization - pricing/quant, execution, risk & PnL
    Practicing the platform and API beforehand - exec speed is HUGE


