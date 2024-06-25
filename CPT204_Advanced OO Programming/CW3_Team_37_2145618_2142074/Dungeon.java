import java.util.*;

public class Dungeon {
    private boolean[][] isRoom;        // is v-w a room site?
    private boolean[][] isCorridor;    // is v-w a corridor site?
    private int N;                     // dimension of dungeon
    private final Map<Integer, List<Integer>> adjacencyList = new HashMap<Integer, List<Integer>>();
    private boolean haveCorridor = false;

    // initialize a new dungeon based on the given board
    public Dungeon(char[][] board) {
        N = board.length;
        isRoom     = new boolean[N][N];
        isCorridor = new boolean[N][N];
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if      (board[i][j] == '.') isRoom[i][j] = true;
                else if (board[i][j] == '+'){
                    haveCorridor = true;
                    isCorridor[i][j] = true;
                }
            }
        }
        generateAdjacencyList();
        System.out.println(adjacencyList);
        System.out.println(findShortestPath(0,99));
    }

    // return dimension of dungeon
    public int size() { return N; }

    // does v correspond to a corridor site? 
    public boolean isCorridor(Site v) {
        int i = v.i();
        int j = v.j();
        if (i < 0 || j < 0 || i >= N || j >= N) return false;
        return isCorridor[i][j];
    }

    // does v correspond to a corridor site? (use int)
    public boolean isCorridor(int i, int j) {
        if (i < 0 || j < 0 || i >= N || j >= N) return false;
        return isCorridor[i][j];
    }

    // does v correspond to a room site?
    public boolean isRoom(Site v) {
        int i = v.i();
        int j = v.j();
        if (i < 0 || j < 0 || i >= N || j >= N) return false;
        return isRoom[i][j];
    }

    // does v correspond to a room site?(use int)
    public boolean isRoom(int i, int j) {
        if (i < 0 || j < 0 || i >= N || j >= N) return false;
        return isRoom[i][j];
    }

    // does v correspond to a wall site?
    public boolean isWall(Site v) {
        return (!isRoom(v) && !isCorridor(v));
    }

    //have Corridor
    public boolean haveCorridor() {
        return haveCorridor;
    }

    // does v-w correspond to a legal move?
    public boolean isLegalMove(Site v, Site w) {
        int i1 = v.i();
        int j1 = v.j();
        int i2 = w.i();
        int j2 = w.j();

        // Judgment does not go beyond the boundary
        if (isNotValidCoordinate(i1, j1) || isNotValidCoordinate(i2, j2)) {
            return false;
        }

        // Judge not to go out of the wal
        if (isWall(v) || isWall(w)) {
            return false;
        }

        // Calculating Manhattan distance
        int manhattanDistance = Math.abs(i1 - i2) + Math.abs(j1 - j2);

        if (manhattanDistance == 1) {
            // 水平或垂直移动
            return (isRoom(v) && isRoom(w)) ||
                    (isCorridor(v) && isRoom(w)) ||
                    (isCorridor(v) && isCorridor(w)) ||
                    (isRoom(v) && isCorridor(w));
        } else if (manhattanDistance == 2) {
            // Move diagonally
            return isRoom(v) && isRoom(w);
        }else return manhattanDistance == 0;
    }

    private boolean isNotValidCoordinate(int i, int j) {
        return i < 0 || j < 0 || i >= N || j >= N;
    }


    private void generateAdjacencyList() {
        int[][] dirs = {
                {-1, 0}, {1, 0}, {0, -1}, {0, 1},{-1, -1}, {-1, 1}, {1, -1}, {1, 1}
        };

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                adjacencyList.putIfAbsent(i*N+j, new ArrayList<>());
                for (int[] d : dirs) {
                    int ni = i + d[0];
                    int nj = j + d[1];
                    if (ni >= 0 && ni < N && nj >= 0 && nj < N) {
                        if (isLegalMove(new Site(i,j), new Site(ni,nj))) {
                            adjacencyList.get(i*N+j).add(ni*N+nj);
                        }
                    }
                }
            }
        }
    }

    // Method to find the shortest path using BFS with priority on movement direction
    public List<Integer> findShortestPath(int start, int end) {
        if (start == end) {
            return Collections.singletonList(start);
        }

        Queue<Integer> queue = new LinkedList<>();
        Map<Integer, Integer> parent = new HashMap<>();
        Set<Integer> visited = new HashSet<>();

        queue.add(start);
        visited.add(start);

        int startX = start / N;
        int startY = start % N;
        int endX = end / N;
        int endY = end % N;

        boolean prioritizeStraight = (startX == endX) || (startY == endY);

        while (!queue.isEmpty()) {
            int current = queue.poll();
            int currentX = current / N;
            int currentY = current % N;

            List<Integer> neighbors = adjacencyList.getOrDefault(current, Collections.emptyList());

            // Separate straight and diagonal neighbors
            List<Integer> straightNeighbors = new ArrayList<>();
            List<Integer> diagonalNeighbors = new ArrayList<>();

            for (int neighbor : neighbors) {
                int neighborX = neighbor / N;
                int neighborY = neighbor % N;

                if (Math.abs(currentX - neighborX) + Math.abs(currentY - neighborY) == 1) {
                    straightNeighbors.add(neighbor);
                } else {
                    diagonalNeighbors.add(neighbor);
                }
            }

            // Process neighbors based on priority
            if (prioritizeStraight) {
                neighbors = straightNeighbors;
                neighbors.addAll(diagonalNeighbors);
            } else {
                neighbors = diagonalNeighbors;
                neighbors.addAll(straightNeighbors);
            }

            for (int neighbor : neighbors) {
                if (!visited.contains(neighbor)) {
                    visited.add(neighbor);
                    parent.put(neighbor, current);
                    queue.add(neighbor);
                    if (neighbor == end) {
                        return buildPath(parent, start, end);
                    }
                }
            }
        }
        return Collections.emptyList(); // Return empty list if no path is found
    }

    // Method to find the path that maximizes the distance from the monster when no corridor
    public List<Integer> findPathAwayFromMonster1(int rogueStart, int monsterPosition) {
        Queue<Integer> queue = new LinkedList<>();
        Map<Integer, Integer> parent = new HashMap<>();
        Set<Integer> visited = new HashSet<>();

        queue.add(rogueStart);
        visited.add(rogueStart);

        // Collect monster adjacent positions
        Set<Integer> monsterAdjacent = new HashSet<>();
        for (int neighbor : adjacencyList.getOrDefault(monsterPosition, Collections.emptyList())) {
            monsterAdjacent.add(neighbor);
        }

        while (!queue.isEmpty()) {
            int current = queue.poll();
            for (int neighbor : adjacencyList.getOrDefault(current, Collections.emptyList())) {
                if (!visited.contains(neighbor) && neighbor != monsterPosition && !monsterAdjacent.contains(neighbor)) {
                    visited.add(neighbor);
                    parent.put(neighbor, current);
                    queue.add(neighbor);
                }
            }
        }

        // Find the farthest node from the monster
        int farthestNode = rogueStart;
        int maxDistance = calculateManhattanDistance(indexToSite(rogueStart), indexToSite(monsterPosition));
        for (int node : visited) {
            int distance = calculateManhattanDistance(indexToSite(node), indexToSite(monsterPosition));
            if (distance > maxDistance) {
                maxDistance = distance;
                farthestNode = node;
            }
        }

        return buildPath(parent, rogueStart, farthestNode);
    }

    public List<Integer> findPathToNearestCorridor(int start, int monster) {
        Queue<Integer> queue = new LinkedList<>();
        Map<Integer, Integer> parent = new HashMap<>();
        Set<Integer> visited = new HashSet<>();

        queue.add(start);
        visited.add(start);

        while (!queue.isEmpty()) {
            int current = queue.poll();
            int currentX = current / N;
            int currentY = current % N;

            // Check if the current position is a corridor
            if (isCorridor(new Site(currentX, currentY))) {
                return buildPath(parent, start, current);
            }

            for (int neighbor : adjacencyList.getOrDefault(current, Collections.emptyList())) {
                if (!visited.contains(neighbor)) {
                    // Check if the neighbor is near the monster
                    boolean nearMonster = isNearMonster(neighbor, monster);

                    // Check if the neighbor is a corridor and not the monster's position
                    boolean isCorridorNeighbor = isCorridor(new Site(neighbor / N, neighbor % N)) && neighbor != monster;

                    if (!nearMonster || isCorridorNeighbor) {
                        visited.add(neighbor);
                        parent.put(neighbor, current);
                        queue.add(neighbor);
                    }
                }
            }
        }
        return Collections.emptyList(); // Return empty list if no corridor is found
    }

    // Helper method to check if a position is near the monster
    private boolean isNearMonster(int position, int monster) {
        int monsterX = monster / N;
        int monsterY = monster % N;
        int posX = position / N;
        int posY = position % N;

        // Check if the position is adjacent to the monster
        return Math.abs(monsterX - posX) <= 1 && Math.abs(monsterY - posY) <= 1;
    }


    // Helper method to build the path from parent map
    private List<Integer> buildPath(Map<Integer, Integer> parent, int start, int end) {
        List<Integer> path = new LinkedList<>();
        for (Integer at = end; at != null; at = parent.get(at)) {
            path.add(at);
        }
        Collections.reverse(path);
        if (!path.isEmpty() && path.get(0) == start) {
            path.remove(0); // Remove the start position from the path
        }
        return path;
    }

    private Site indexToSite(int index) {
        int i = index / N;
        int j = index % N;
        return new Site(i, j);
    }

    private int siteToIndex(Site site) {
        return site.i() * N + site.j();
    }

    private int calculateManhattanDistance(Site a, Site b) {
        return Math.abs(a.i() - b.i()) + Math.abs(a.j() - b.j());
    }

}
