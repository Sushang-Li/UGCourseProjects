public class Site {
    private int i;
    private int j;

    /**
     * Initializes a Site with coordinates (i, j).
     *
     * @param i the row coordinate
     * @param j the column coordinate
     */
    public Site(int i, int j) {
        this.i = i;
        this.j = j;
    }

    /**
     * Returns the row coordinate of the site.
     *
     * @return the row coordinate
     */
    public int i() {
        return i;
    }

    /**
     * Returns the column coordinate of the site.
     *
     * @return the column coordinate
     */
    public int j() {
        return j;
    }

    /**
     * Returns a string representation of the site.
     *
     * @return a string in the format "Site{i=x, j=y}"
     */
    @Override
    public String toString() {
        return "Site{" +
                "i=" + i +
                ", j=" + j +
                '}';
    }

    /**
     * Calculates the Manhattan distance between the invoking Site and the given Site w.
     * The Manhattan distance is the sum of the absolute differences of their coordinates.
     *
     * @param w the site to which the distance is calculated
     * @return the Manhattan distance between this site and site w
     */
    public int manhattanTo(Site w) {
        Site v = this;
        int i1 = v.i();
        int j1 = v.j();
        int i2 = w.i();
        int j2 = w.j();
        return Math.abs(i1 - i2) + Math.abs(j1 - j2);
    }

    /**
     * Checks if the invoking site is equal to the given site w.
     * Two sites are considered equal if their Manhattan distance is zero.
     *
     * @param w the site to compare with
     * @return true if the sites are equal, false otherwise
     */
    public boolean equals(Site w) {
        return (manhattanTo(w) == 0);
    }
}
