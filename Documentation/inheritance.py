import matplotlib.pyplot as plt
import networkx as nx

# Define the inheritance relationships
edges = [
    ("ax_type", "T2"),
    ("ax_type2", "T1"),
    ("ax_type2", "T2"),
    ("ax_name_container", "TBase"),
    ("ax_name_container_base", "ax_name_container_base_const"),
    ("ntuple", "ntuple_base_t<T>..."),
]

# Create directed graph
G = nx.DiGraph()
G.add_edges_from(edges)

# Position nodes using spring layout
pos = nx.spring_layout(G, seed=42)

# Draw the graph
plt.figure(figsize=(10, 6))
nx.draw(G, pos, with_labels=True, node_size=3000, node_color='lightgray', font_size=10, arrows=True)
plt.title("Inheritance Diagram from ntuples.hh")
plt.tight_layout()
plt.show()
