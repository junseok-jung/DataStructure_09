#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTICES 10
#define TARGET_EDGES 20
#define INF 99999

// 그래프 구조체
typedef struct {
    int weight[MAX_VERTICES][MAX_VERTICES];
    int visited[MAX_VERTICES];
} Graph;

// 그래프 초기화
void initGraph(Graph* g) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        g->visited[i] = 0;
        for (int j = 0; j < MAX_VERTICES; j++) {
            if (i == j) {
                g->weight[i][j] = 0;
            }
            else {
                g->weight[i][j] = INF;
            }
        }
    }
}

// 깊이 우선 탐색
void dfs(Graph* g, int v) {
    g->visited[v] = 1;
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (g->weight[v][i] != 0 && g->weight[v][i] != INF && g->visited[i] == 0) {
            dfs(g, i);
        }
    }
}

// 연결 그래프 여부 확인
int isConnected(Graph* g) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        g->visited[i] = 0;
    }

    dfs(g, 0);

    for (int i = 0; i < MAX_VERTICES; i++) {
        if (g->visited[i] == 0) {
            return 0;
        }
    }
    return 1;
}

// 무작위 연결 그래프 생성
void generateRandomConnectedGraph(Graph* g) {
    while (1) {
        initGraph(g);
        int edge_count = 0;

        while (edge_count < TARGET_EDGES) {
            int u = rand() % MAX_VERTICES;
            int v = rand() % MAX_VERTICES;

            // 자기 루프 및 중복 간선 제외
            if (u != v && g->weight[u][v] == INF) {
                int w = (rand() % 100) + 1;
                g->weight[u][v] = w;
                g->weight[v][u] = w;
                edge_count++;
            }
        }

        if (isConnected(g) == 1) {
            break;
        }
    }
}

// 생성된 그래프 출력
void printGraph(Graph* g) {
    printf("Graph\n");
    printf("Vertices : %d\n", MAX_VERTICES);
    printf("Edges    : %d\n\n", TARGET_EDGES);

    printf("Edges\n");
    for (int u = 0; u < MAX_VERTICES; u++) {
        for (int v = u + 1; v < MAX_VERTICES; v++) {
            if (g->weight[u][v] != INF) {
                printf("(%d, %d) Weight: %d\n", u, v, g->weight[u][v]);
            }
        }
    }
    printf("\n");
}

// 모든 정점 쌍 최단경로 계산 및 출력 (Floyd-Warshall)
void findAndPrintShortestPaths(Graph* g) {
    int dist[MAX_VERTICES][MAX_VERTICES];
    int next_vertex[MAX_VERTICES][MAX_VERTICES];

    // 거리 및 다음 정점 배열 초기화
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            dist[i][j] = g->weight[i][j];
            if (i != j && g->weight[i][j] != INF) {
                next_vertex[i][j] = j;
            }
            else {
                next_vertex[i][j] = -1;
            }
        }
    }

    // 최단거리 및 경로 갱신
    for (int k = 0; k < MAX_VERTICES; k++) {
        for (int i = 0; i < MAX_VERTICES; i++) {
            for (int j = 0; j < MAX_VERTICES; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next_vertex[i][j] = next_vertex[i][k];
                }
            }
        }
    }

    // 45개 정점 쌍 출력
    printf("Shortest Paths\n\n");
    int pair_count = 0;

    for (int u = 0; u < MAX_VERTICES; u++) {
        for (int v = u + 1; v < MAX_VERTICES; v++) {
            printf("%d -> %d\n", u, v);
            printf("Path     : %d", u);

            int curr = u;
            while (curr != v) {
                curr = next_vertex[curr][v];
                printf(" -> %d", curr);
            }
            printf("\n");
            printf("Distance : %d\n\n", dist[u][v]);
            pair_count++;
        }
    }

    printf("Total vertex pairs : %d\n", pair_count);
}

int main(void) {
    srand((unsigned int)time(NULL));

    Graph g;
    generateRandomConnectedGraph(&g);
    printGraph(&g);
    findAndPrintShortestPaths(&g);

    return 0;
}