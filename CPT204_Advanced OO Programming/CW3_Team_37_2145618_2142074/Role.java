/**
 * Abstract class Role, representing a character in the game.
 * This class defines an abstract method move(), to be implemented by specific character classes.
 */
abstract class Role {

    /**
     * Abstract method move, which defines the movement behavior of the character.
     * Specific character classes must implement this method to provide their unique movement logic.
     *
     * @return The new position of the character after moving, of type Site.
     */
    abstract Site move();
}
