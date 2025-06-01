import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

# Configurações do gráfico
plt.style.use('seaborn-v0_8')
plt.rcParams['figure.dpi'] = 120
plt.rcParams['font.size'] = 10

# Carregar os dados
file_path = 'Binary-Tree/RB_Resultados/rb_resultados.csv'
try:
    df = pd.read_csv(file_path)
except FileNotFoundError:
    raise FileNotFoundError("Arquivo não encontrado!")

# Verificar colunas necessárias
required_cols = ['tamanho', 'TempoConstrucao(ms)', 'BuscaExistente(ns)',
                'BuscaInexistente(ns)', 'RemocaoExistente(ns)', 'RemocaoInexistente(ns)']
missing_cols = set(required_cols) - set(df.columns)
if missing_cols:
    raise ValueError(f"Colunas faltando: {missing_cols}")

# Converter APENAS tempo de construção de μs para ms quando tamanho=1000
if 1000 in df['tamanho'].values:
    mask = df['tamanho'] == 1000
    df.loc[mask, 'TempoConstrucao(ms)'] = df.loc[mask, 'TempoConstrucao(ms)'] / 1000  # μs → ms

# Calcular médias
medias = df.groupby('tamanho').mean()
tamanhos = medias.index.astype(int)

# Criar figura com eixos separados
fig, ax1 = plt.subplots(figsize=(14, 7))
ax2 = ax1.twinx()  # Cria um segundo eixo Y

# Posições equidistantes no eixo X
x_positions = np.arange(len(tamanhos))

# Estilos para cada métrica
styles = {
    'TempoConstrucao(ms)': {
        'label': 'Construção (ms)',
        'color': '#1f77b4',
        'marker': 'o',
        'ls': '-',
        'axis': ax1
    },
    'BuscaExistente(ns)': {
        'label': 'Busca Existente (ns)',
        'color': '#ff7f0e',
        'marker': 's',
        'ls': '--',
        'axis': ax2
    },
    'BuscaInexistente(ns)': {
        'label': 'Busca Inexistente (ns)',
        'color': '#2ca02c',
        'marker': 'D',
        'ls': '-.',
        'axis': ax2
    },
    'RemocaoExistente(ns)': {
        'label': 'Remoção Existente (ns)',
        'color': '#d62728',
        'marker': '^',
        'ls': ':',
        'axis': ax2
    },
    'RemocaoInexistente(ns)': {
        'label': 'Remoção Inexistente (ns)',
        'color': '#9467bd',
        'marker': 'v',
        'ls': '-',
        'axis': ax2
    }
}

# Plotar cada métrica
for col, params in styles.items():
    if col in medias.columns:
        params['axis'].plot(x_positions, medias[col],
                          label=params['label'],
                          color=params['color'],
                          marker=params['marker'],
                          linestyle=params['ls'],
                          linewidth=2,
                          markersize=8)

# Configurar eixos
ax1.set_xticks(x_positions)
ax1.set_xticklabels(tamanhos, rotation=45, ha='right')
ax1.set_xlabel('Tamanho da Estrutura', fontsize=12)
ax1.set_ylabel('Tempo de Construção (ms)', fontsize=12, color='#1f77b4')
ax2.set_ylabel('Tempo de Operações (ns)', fontsize=12, color='#ff7f0e')

# Estilização adicional
ax1.tick_params(axis='y', colors='#1f77b4')
ax2.tick_params(axis='y', colors='#ff7f0e')
ax1.set_title('Desempenho da Árvore RB', pad=20, fontsize=14)
ax1.grid(True, linestyle='--', alpha=0.3)

# Unificar legendas
lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax1.legend(lines1 + lines2, labels1 + labels2,
          bbox_to_anchor=(1.1, 1), loc='upper left')

# Salvar e mostrar
plt.tight_layout()
output_dir = os.path.dirname(file_path)
os.makedirs(output_dir, exist_ok=True)
plt.savefig(os.path.join(output_dir, 'desempenho_avl.png'),
           dpi=300, bbox_inches='tight')
plt.show()