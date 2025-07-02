import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.collections import LineCollection
import numpy as np
from matplotlib.font_manager import FontProperties

def read_dcel_file(filename):
    with open(filename, 'r') as f:
        lines = [line.strip() for line in f.readlines() if line.strip()]
    
    # Read header
    n_vertices, n_edges, n_faces = map(int, lines[0].split())
    
    # Read vertices
    vertices = []
    vertex_edges = {}  # To store starting half-edge for each vertex
    for i in range(1, n_vertices + 1):
        parts = list(map(int, lines[i].split()))
        x, y = parts[0], parts[1]
        edge = parts[2] if len(parts) > 2 else None
        vertices.append((x, y))
        vertex_edges[i] = edge
    
    # Read faces
    faces = {}
    face_start = n_vertices + 1
    for i in range(face_start, face_start + n_faces):
        face_id = i - face_start + 1
        edge_id = int(lines[i])
        faces[face_id] = edge_id
    
    # Read half-edges
    half_edges = {}
    edge_start = face_start + n_faces
    for i in range(edge_start, edge_start + 2 * n_edges):
        parts = list(map(int, lines[i].split()))
        edge_id = i - edge_start + 1
        half_edges[edge_id] = {
            'origin': parts[0],
            'twin': parts[1],
            'face': parts[2],
            'next': parts[3],
            'prev': parts[4]
        }
    
    return vertices, vertex_edges, faces, half_edges

def visualize_dcel(vertices, vertex_edges, faces, half_edges):
    fig, ax = plt.subplots(figsize=(14, 12))
    ax.set_aspect('equal')
    
    # Calculate automatic limits with padding
    xs = [v[0] for v in vertices]
    ys = [v[1] for v in vertices]
    x_pad = (max(xs) - min(xs)) * 0.2
    y_pad = (max(ys) - min(ys)) * 0.2
    ax.set_xlim(min(xs) - x_pad, max(xs) + x_pad)
    ax.set_ylim(min(ys) - y_pad, max(ys) + y_pad)
    
    # Create color maps
    face_colors = plt.cm.tab20.colors
    edge_colors = plt.cm.Set2.colors
    
    # Draw faces first (background)
    for face_id, start_edge in faces.items():
        polygon = []
        edge = start_edge
        visited = set()
        
        while edge not in visited:
            visited.add(edge)
            origin = half_edges[edge]['origin']
            polygon.append(vertices[origin-1])
            edge = half_edges[edge]['next']
        
        poly = patches.Polygon(polygon, closed=True, 
                             alpha=0.2, color=face_colors[face_id % len(face_colors)],
                             label=f'Face {face_id}')
        ax.add_patch(poly)
    
    # Draw all edges (gray lines)
    lines = []
    for edge_id, edge_data in half_edges.items():
        origin = edge_data['origin']
        twin = edge_data['twin']
        start = vertices[origin-1]
        end = vertices[half_edges[twin]['origin']-1] if twin in half_edges else start
        lines.append([start, end])
    
    lc = LineCollection(lines, colors='gray', linewidths=2, alpha=0.5, zorder=1)
    ax.add_collection(lc)
    
    # Draw half-edges with direction and color coding
    arrowprops = dict(arrowstyle="->", lw=1.5, alpha=0.8, 
                     shrinkA=5, shrinkB=5, mutation_scale=12)
    
    for edge_id, edge_data in half_edges.items():
        origin = edge_data['origin']
        twin = edge_data['twin']
        face = edge_data['face']
        
        start = np.array(vertices[origin-1])
        end = np.array(vertices[half_edges[twin]['origin']-1]) if twin in half_edges else start
        
        # Draw colored direction arrow
        ax.annotate('', xy=end, xytext=start,
                   arrowprops=dict(**arrowprops, color=edge_colors[face % len(edge_colors)]))
        
        # Label the half-edge
        midpoint = (start + end) / 2
        offset = (end - start) / np.linalg.norm(end - start) * 0.3 if np.linalg.norm(end - start) > 0 else 0
        label_pos = midpoint + np.array([-offset[1], offset[0]]) * 0.5  # Perpendicular offset
        
        ax.text(label_pos[0], label_pos[1], f'e{edge_id}', 
                fontsize=9, ha='center', va='center',
                bbox=dict(facecolor='white', alpha=0.8, edgecolor='none', pad=2),
                color=edge_colors[face % len(edge_colors)], weight='bold')
    
    # Draw vertices with their starting half-edge
    for i, (x, y) in enumerate(vertices, 1):
        ax.plot(x, y, 'o', markersize=12, markeredgecolor='black', markerfacecolor='white', zorder=4)
        ax.text(x, y, f'V{i}\n(se:{vertex_edges.get(i, "?")})', 
                ha='center', va='center', fontsize=9,
                bbox=dict(facecolor='white', alpha=0.7, edgecolor='none', pad=1))
    
    # Create custom legend
    font = FontProperties()
    font.set_family('monospace')
    
    legend_elements = [
        patches.Patch(facecolor=face_colors[i%len(face_colors)], alpha=0.3, 
                     label=f'Face {i} (e{faces[i]})') for i in faces
    ]
    
    ax.legend(handles=legend_elements, loc='upper right', prop=font,
              title="Faces (semi-aresta representante)")
    
    # Add twin edge connections information
    twin_info = []
    for edge_id, edge_data in half_edges.items():
        if edge_id < edge_data['twin']:  # Avoid duplicate entries
            twin_info.append(f"e{edge_id} ↔ e{edge_data['twin']}")
    
    ax.text(0.02, 0.98, "\n".join(twin_info[:10]),  # Show first 10 pairs
            transform=ax.transAxes, ha='left', va='top',
            bbox=dict(facecolor='white', alpha=0.7), fontsize=8)
    
    plt.title('Visualização DCEL Detalhada\n(Semi-arestas coloridas por face, setas mostram direção)', pad=20)
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print("Usage: python dcel_visualizer.py <input_file.dat>")
        sys.exit(1)
    
    input_file = sys.argv[1]
    vertices, vertex_edges, faces, half_edges = read_dcel_file(input_file)
    visualize_dcel(vertices, vertex_edges, faces, half_edges)
