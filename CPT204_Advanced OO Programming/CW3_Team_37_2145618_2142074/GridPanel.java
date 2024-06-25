import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

/**
 * GridPanel class extends JPanel and implements KeyListener to handle keyboard events.
 * This class creates a grid of cells to represent the game board and allows user interaction through keyboard input.
 */
class GridPanel extends JPanel implements KeyListener {
    private JPanel[][] gridCells;  // 2D array of JPanel objects representing the grid cells
    private Game game;  // Reference to the Game object

    /**
     * Constructor for the GridPanel class.
     * Initializes the grid with specified size and sets up the key listener.
     *
     * @param gridSize The size of the grid (number of rows and columns)
     * @param game The Game object associated with this grid
     */
    public GridPanel(int gridSize, Game game) {
        this.game = game;
        setLayout(new GridLayout(gridSize, gridSize));
        gridCells = new JPanel[gridSize][gridSize];
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                JPanel cell = new JPanel();
                cell.setBorder(BorderFactory.createLineBorder(Color.BLACK));
                cell.setOpaque(true);
                gridCells[i][j] = cell;
                add(cell);
            }
        }
        // To ensure JPanel can receive keyboard events, it needs to be focusable
        this.setFocusable(true);
        this.requestFocusInWindow();
        addKeyListener(this);
    }

    /**
     * Sets the color of a specific cell in the grid.
     *
     * @param row The row index of the cell
     * @param col The column index of the cell
     * @param color The color to set the cell background
     */
    public void setCellColor(int row, int col, Color color) {
        if (isValidCell(row, col)) {
            gridCells[row][col].setBackground(color);
        }
    }

    /**
     * Checks if the specified cell position is valid within the grid bounds.
     *
     * @param row The row index of the cell
     * @param col The column index of the cell
     * @return True if the cell position is valid, otherwise false
     */
    private boolean isValidCell(int row, int col) {
        return row >= 0 && row < gridCells.length && col >= 0 && col < gridCells[0].length;
    }

    @Override
    public void keyTyped(KeyEvent e) {
        // Do nothing
    }

    @Override
    public void keyPressed(KeyEvent e) {
        int keyCode = e.getKeyCode();
        if ((keyCode >= KeyEvent.VK_1 && keyCode <= KeyEvent.VK_9) ||
                (keyCode >= KeyEvent.VK_NUMPAD1 && keyCode <= KeyEvent.VK_NUMPAD9)) {
            int nextMove;
            if (keyCode >= KeyEvent.VK_NUMPAD1 && keyCode <= KeyEvent.VK_NUMPAD9) {
                nextMove = keyCode - KeyEvent.VK_NUMPAD0;
            } else {
                nextMove = keyCode - KeyEvent.VK_0;
            }
            game.processMove(nextMove);
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        // Do nothing
    }
}
