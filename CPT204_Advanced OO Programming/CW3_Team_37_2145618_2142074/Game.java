import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

public class Game {

    // portable newline
    private final static String NEWLINE = System.getProperty("line.separator");

    private Dungeon dungeon;     // the dungeon
    private char MONSTER;        // name of the monster (A - Z)
    private char ROGUE = '@';    // name of the rogue
    private int N;               // board dimension
    private Site monsterSite;    // location of monster
    private Site rogueSite;      // location of rogue
    private Role monster;     // the monster
    private Role rogue;         // the rogue
    private int mode;            // game mode
    private GridPanel gridPanel;
    private JFrame gridFrame;
    private boolean waitingForInput;
    private ArrayList<Site> roguePath = new ArrayList<Site>();

    // initialize board from file
    public Game(In in) {
        // read in data
        N = Integer.parseInt(in.readLine());
        char[][] board = new char[N][N];
        for (int i = 0; i < N; i++) {
            String s = in.readLine();
            for (int j = 0; j < N; j++) {
                board[i][j] = s.charAt(2*j);

                // check for monster's location
                if (board[i][j] >= 'A' && board[i][j] <= 'Z') {
                    MONSTER = board[i][j];
                    board[i][j] = '.';
                    monsterSite = new Site(i, j);
                }

                // check for rogue's location
                if (board[i][j] == ROGUE) {
                    board[i][j] = '.';
                    rogueSite  = new Site(i, j);
                    roguePath.add(rogueSite);
                }
            }
        }
        dungeon = new Dungeon(board);
        monster = new Monster(this);
        rogue   = new Rogue(this);
    }

    // return position of monster and rogue
    public Site getMonsterSite() { return monsterSite; }
    public Site getRogueSite()   { return rogueSite;   }
    public Dungeon getDungeon()  { return dungeon;     }

    public void setGridPanel(GridPanel gridPanel) {
        this.gridPanel = gridPanel;
    }

    // play until monster catches the rogue
    public void play(GridPanel gridPanel, JFrame gridFrame, int mode) {
        this.gridPanel = gridPanel;
        this.gridFrame = gridFrame;
        this.mode = mode;

        // Use SwingWorker to update the grid panel in the background
        SwingWorker<Void, Void> worker = new SwingWorker<Void, Void>() {
            @Override
            protected Void doInBackground() {
                for (int t = 1; true; t++) {
                    updateGridPanel();
                    try {
                        Thread.sleep(200);
                        // Check for end condition
                        if (monsterSite.equals(rogueSite)) break;
                        // Monster moves
                        if (mode == 1) {
                            waitingForInput = true;
                            while (waitingForInput) {
                                Thread.sleep(100); // Wait for user input
                            }
                        } else {
                            Site next = monster.move();
                            if (dungeon.isLegalMove(monsterSite, next)) {
                                monsterSite = next;
                            } else {
                                throw new RuntimeException("Monster can't catch rogue anyway");
                            }
                        }
                        updateGridPanel();
                        Thread.sleep(200);
                        // Rogue moves
                        if (monsterSite.equals(rogueSite)) break;

                        if (mode == 2) {
                            waitingForInput = true;
                            while (waitingForInput) {
                                Thread.sleep(100); // Wait for user input
                            }
                        } else {
                            Site next = rogue.move();
                            if (dungeon.isLegalMove(rogueSite, next)) {
                                rogueSite = next;
                            } else {
                                throw new RuntimeException("Monster can't catch rogue anyway");
                            }
                        }
                        updateGridPanel();
                        SwingUtilities.invokeLater(() -> System.out.println(Game.this.toString()));
                        Thread.sleep(500);

                    } catch (InterruptedException e) {
                        System.out.println(e.getMessage());
                    } catch (RuntimeException e) {
                        JOptionPane.showMessageDialog(gridFrame, e.getMessage());
                    }
                }
                JOptionPane.showMessageDialog(gridFrame, "Caught by monster");
                return null;
            }
        };
        worker.execute();
    }

    public void processMove(int nextMove) {
        Site next = moveToSite(nextMove);
        if (mode==1&&dungeon.isLegalMove(monsterSite, next)) {
            monsterSite = next;
            waitingForInput = false;
        } else if (mode==2&&dungeon.isLegalMove(rogueSite, next)) {
            rogueSite = next;
            waitingForInput = false;
        } else {
            throw new RuntimeException("Illegal move");
        }
    }

    private void updateGridPanel() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (dungeon.isCorridor(i, j)) {
                    gridPanel.setCellColor(i, j, new Color(155, 128, 40));
                } else if (dungeon.isRoom(i, j)) {
                    gridPanel.setCellColor(i, j, new Color(99, 155, 40));
                } else {
                    gridPanel.setCellColor(i, j, new Color(0, 0, 0));
                }
            }
        }
        gridPanel.setCellColor(rogueSite.i(), rogueSite.j(), new Color(17, 56, 184));
        gridPanel.setCellColor(monsterSite.i(), monsterSite.j(), new Color(184, 17, 42));
        gridPanel.repaint();
    }

    // move(intInput2Site)
    public Site moveToSite(int i) {
        int[][] dirs = {
                {1, -1},  // 1: Southwest
                {1, 0},   // 2: South
                {1, 1},   // 3: Southeast
                {0, -1},  // 4: West
                {0, 0},   // 5: Middle
                {0, 1},   // 6: East
                {-1, -1}, // 7: Northwest
                {-1, 0},  // 8: North
                {-1, 1}   // 9: Northeast
        };
        Site now;
        if(mode==1){
            now = monsterSite;
        }else {
            now = rogueSite;
        }

        int ni = now.i() + dirs[i - 1][0];
        int nj = now.j() + dirs[i - 1][1];
        return new Site(ni, nj);
    }

    // string representation of game state (inefficient because of Site and string concat)
    public String toString() {
        String s = "";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                Site site = new Site(i, j);
                if (rogueSite.equals(monsterSite) && (rogueSite.equals(site))) s += "* ";
                else if (rogueSite.equals(site)) s += ROGUE + " ";
                else if (monsterSite.equals(site)) s += MONSTER + " ";
                else if (dungeon.isRoom(site)) s += ". ";
                else if (dungeon.isCorridor(site)) s += "+ ";
                else if (dungeon.isRoom(site)) s += ". ";
                else if (dungeon.isWall(site)) s += "  ";
            }
            s += NEWLINE;
        }
        return s;
    }
}
