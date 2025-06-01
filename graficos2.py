import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

# Configurações do gráfico
plt.style.use('seaborn-v0_8')
plt.rcParams['figure.dpi'] = 120
plt.rcParams['font.size'] = 10

# Carregar ambos arquivos CSV
avl_path = 'Binary-Tree/AVL_Resultados/tree_avl_resultados.csv'
rb_path = 'Binary-Tree/RB_Resultados/rb_resultados.csv'

def carregar_dados(filepath, tipo_arvore):
    df = pd.read_csv(filepath)
    
    # Converter APENAS tempo de construção de μs para ms quando tamanho=1000
    if 1000 in df['tamanho'].values:
        mask = df['tamanho'] == 1000
        df.loc[mask, 'TempoConstrucao(ms)'] = df.loc[mask, 'TempoConstrucao(ms)'] / 1000
    
    df['tipo_arvore'] = tipo_arvore  # Adiciona coluna identificadora
    return df

try:
    df_avl = carregar_dados(avl_path, 'AVL')
    df_rb = carregar_dados(rb_path, 'RB')
    df = pd.concat([df_avl, df_rb])  # Combina os dados
except FileNotFoundError as e:
    raise FileNotFoundError(f"Arquivo não encontrado: {e}")

# Verificar colunas necessárias
required_cols = ['tamanho', 'TempoConstrucao(ms)', 'BuscaExistente(ns)',
                'BuscaInexistente(ns)', 'RemocaoExistente(ns)', 'RemocaoInexistente(ns)']
missing_cols = set(required_cols) - set(df.columns)
if missing_cols:
    raise ValueError(f"Colunas faltando: {missing_cols}")

# Calcular médias por tamanho e tipo de árvore
medias = df.groupby(['tamanho', 'tipo_arvore']).mean().reset_index()
tamanhos = sorted(df['tamanho'].unique())

# Criar figura com eixos separados
fig, ax1 = plt.subplots(figsize=(16, 8))
ax2 = ax1.twinx()

# Posições equidistantes no eixo X
x_positions = np.arange(len(tamanhos))
bar_width = 0.35  # Para gráfico de barras lado a lado

# Estilos para cada métrica e tipo de árvore
styles = {
    'AVL': {
        'color': '#1f77b4',
        'marker': 'o',
        'prefix': 'AVL - '
    },
    'RB': {
        'color': '#ff7f0e',
        'marker': 's',
        'prefix': 'RB - '
    }
}

# Plotar cada operação
for col in ['TempoConstrucao(ms)', 'BuscaExistente(ns)', 
            'BuscaInexistente(ns)', 'RemocaoExistente(ns)', 
            'RemocaoInexistente(ns)']:
    
    for tipo, params in styles.items():
        dados = medias[(medias['tipo_arvore'] == tipo)].sort_values('tamanho')
        
        if col == 'TempoConstrucao(ms)':
            ax1.plot(x_positions, dados[col],
                    label=params['prefix'] + col.split('(')[0],
                    color=params['color'],
                    marker=params['marker'],
                    linestyle='-',
                    linewidth=2,
                    markersize=8)
        else:
            ax2.plot(x_positions, dados[col],
                    label=params['prefix'] + col.split('(')[0],
                    color=params['color'],
                    marker=params['marker'],
                    linestyle='--',
                    linewidth=1.5,
                    markersize=6)

# Configurar eixos
ax1.set_xticks(x_positions)
ax1.set_xticklabels(tamanhos, rotation=45, ha='right')
ax1.set_xlabel('Tamanho da Estrutura', fontsize=12)
ax1.set_ylabel('Tempo de Construção (ms)', fontsize=12, color='#1f77b4')
ax2.set_ylabel('Tempo de Operações (ns)', fontsize=12, color='#ff7f0e')

# Estilização
ax1.tick_params(axis='y', colors='#1f77b4')
ax2.tick_params(axis='y', colors='#ff7f0e')
ax1.set_title('Comparação AVL vs Rubro-Negra', pad=20, fontsize=14)
ax1.grid(True, linestyle='--', alpha=0.3)

# Legendas unificadas
lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax1.legend(lines1 + lines2, labels1 + labels2, 
          bbox_to_anchor=(1.15, 1), loc='upper left')

# Salvar e mostrar
plt.tight_layout()
output_dir = 'Binary-Tree/Comparacoes'
os.makedirs(output_dir, exist_ok=True)
plt.savefig(os.path.join(output_dir, 'comparacao_avl_rb.png'),
           dpi=300, bbox_inches='tight')
plt.show()