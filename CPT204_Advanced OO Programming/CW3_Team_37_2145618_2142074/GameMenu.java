import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FilenameFilter;

public class GameMenu {
    private JFrame frame;
    private Image backgroundImage;
    private JPanel mainPanel;
    private JFrame gridFrame;
    private GridPanel gridPanel;
    private Integer mode;

    public GameMenu() {
        // Load the background image
        backgroundImage = new ImageIcon("background.png").getImage();
        initializeUI();
    }

    /**
     * Initializes the user interface for the game menu.
     */
    private void initializeUI() {
        // Create and set up the main window
        frame = new JFrame("Game Menu");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(400, 600);

        // Set the window's location (e.g., centered on the screen)
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        int x = (screenSize.width - frame.getWidth()) / 2;
        int y = (screenSize.height - frame.getHeight()) / 2;
        frame.setLocation(x, y);

        // Create a custom JPanel to draw the background image
        JPanel backgroundPanel = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), this);
            }
        };
        backgroundPanel.setLayout(new GridBagLayout()); // Use GridBagLayout for button layout
        frame.setContentPane(backgroundPanel);

        mainPanel = new JPanel();
        mainPanel.setOpaque(false);
        mainPanel.setLayout(new GridBagLayout());
        backgroundPanel.add(mainPanel);

        // Create buttons
        JButton option1 = new JButton("1. rogue vs monster");
        JButton option2 = new JButton("2. rogue vs monster (people)");
        JButton option3 = new JButton("3. rogue (people) vs monster");
        JButton option4 = new JButton("4. exit");

        // Create GridBagConstraints to control button layout
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(10, 10, 10, 10); // Set button margins
        gbc.gridx = 0;
        gbc.gridy = 0;
        mainPanel.add(option1, gbc);

        gbc.gridy = 1;
        mainPanel.add(option2, gbc);

        gbc.gridy = 2;
        mainPanel.add(option3, gbc);

        gbc.gridy = 3;
        mainPanel.add(option4, gbc);

        // Add button click event handlers
        option1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                mode = 0;
                showFileList(mode);
            }
        });

        option2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                mode = 1;
                showFileList(mode);
            }
        });

        option3.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                mode = 2;
                showFileList(mode);
            }
        });

        option4.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        });

        // Display the window
        frame.setVisible(true);
    }

    /**
     * Displays a list of files in the "Dungeons" directory.
     *
     * @param mode The game mode selected by the user
     */
    private void showFileList(int mode) {
        File dir = new File("Dungeons");
        if (!dir.exists() || !dir.isDirectory()) {
            JOptionPane.showMessageDialog(frame, "Dungeons Folder does not exist");
            return;
        }

        File[] files = dir.listFiles(new FilenameFilter() {
            @Override
            public boolean accept(File dir, String name) {
                return name.toLowerCase().endsWith(".txt");
            }
        });

        if (files == null || files.length == 0) {
            JOptionPane.showMessageDialog(frame, "There are no files in the Dungeons folder");
            return;
        }

        JList<File> fileList = new JList<>(files);
        fileList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        JScrollPane scrollPane = new JScrollPane(fileList);

        // Create a new window to display the file list
        JFrame listFrame = new JFrame("Dungeon list");
        listFrame.setSize(400, 300);
        listFrame.setLocationRelativeTo(frame);
        listFrame.add(scrollPane);
        listFrame.setVisible(true);

        fileList.addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting()) {
                File selectedFile = fileList.getSelectedValue();
                if (selectedFile != null) {
                    // Get the name of the selected file
                    String fileName = selectedFile.getAbsolutePath();

                    In stdin = new In(fileName);
                    Game game = new Game(stdin);
                    // Display a new window
                    showGridWindow("Current running: " + selectedFile.getName() + "; The map size is: " + game.getDungeon().size(), game.getDungeon().size(), game);
                    System.out.println(game);
                    game.play(gridPanel, gridFrame, mode);
                }
            }
        });
    }

    /**
     * Displays a grid window with the game state.
     *
     * @param message  The message to display at the top of the window
     * @param gridSize The size of the grid
     * @param game     The game instance
     */
    private void showGridWindow(String message, int gridSize, Game game) {
        gridFrame = new JFrame("Game Window");
        gridFrame.setSize(600, 600);
        gridFrame.setLayout(new BorderLayout());

        // Add a label with the message at the top
        JLabel label = new JLabel(message, SwingConstants.CENTER);
        gridFrame.add(label, BorderLayout.NORTH);

        // Create the grid panel
        gridPanel = new GridPanel(gridSize, game);
        game.setGridPanel(gridPanel);

        gridFrame.add(gridPanel, BorderLayout.CENTER);
        gridFrame.setLocationRelativeTo(null);
        gridFrame.setVisible(true);
    }

    public static void main(String[] args) {
        new GameMenu();
    }
}
