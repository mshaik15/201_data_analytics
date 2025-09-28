import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import norm

# Load data
file_name = 'data.csv'
df = pd.read_csv(file_name)

df.columns = df.columns.str.strip()
df = df.rename(columns={df.columns[0]: 'trial', df.columns[1]: 'time'})
df = df.dropna()
df['time'] = df['time'].astype(int)

mean = df['time'].mean()
std = df['time'].std(ddof=1)

print(f'Sample Mean: {mean:.2f} μs') # Sample Mean: 1310125.25 μs
print(f'Sample Standard Deviation: {std:.2f} μs') # Sample Standard Deviation: 107202.17 μs

plt.figure(figsize=(10, 6))
plt.hist(df["time"], bins=15, density=True, alpha=0.6, color='skyblue', edgecolor="black")

# Normal distribution curve
x_min, x_max = plt.xlim()
x = np.linspace(x_min, x_max, 1000)
p = norm.pdf(x, mean, std)
plt.plot(x, p, 'r', linewidth=2, label='Normal Distribution')

# Histogram
plt.title('Histogram of Ball Roll Times with Normal Distribution Fit')
plt.xlabel('Time (μs)')
plt.ylabel('Relative Frequency')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plt.savefig("histogram.pdf")
plt.show()
plt.close()

# Time Series
plt.figure(figsize=(10,6))
plt.plot(df["trial"], df["time"], marker="o", linestyle="-", alpha=0.7)
plt.title("Measurement Time vs Trial Number")
plt.xlabel("Trial Number")
plt.ylabel("Time (μs)")
plt.grid(True, linestyle="--", alpha=0.6)
plt.tight_layout()
plt.savefig("timeseries.pdf")
plt.show()
plt.close()
