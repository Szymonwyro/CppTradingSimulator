import pandas as pd
import matplotlib.pyplot as plt

# Load the simulated price data
df = pd.read_csv('price_simulation.csv')

# Plot the price over time
plt.figure(figsize=(12, 6))

plt.plot(df['Day'], df['Mid'], label='Mid Price', color='blue')
plt.plot(df['Day'], df['Bid'], label='Bid Price', color='green', linestyle='--')
plt.plot(df['Day'], df['Ask'], label='Ask Price', color='red', linestyle='--')

plt.title('Simulated Market Prices Over Time')
plt.xlabel('Day')
plt.ylabel('Price')

plt.grid(True)
plt.legend()

plt.show()
