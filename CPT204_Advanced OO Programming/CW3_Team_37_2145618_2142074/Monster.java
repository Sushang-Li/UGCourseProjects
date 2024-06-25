import java.util.List;

public class Monster extends Role{
    private Game game;
    private Dungeon dungeon;
    private int N;

    public Monster(Game game) {
        this.game    = game;
        this.dungeon = game.getDungeon();
        this.N       = dungeon.size();
    }

    // move function for monster
    @Override
    public Site move() {
        int mi = game.getMonsterSite().i();
        int mj = game.getMonsterSite().j();
        int ri = game.getRogueSite().i();
        int rj = game.getRogueSite().j();
        Site rogue   = game.getRogueSite();
        Site move    = null;

        // The monster will move to the nearest shortest path to the rogue
        List<Integer> path = dungeon.findShortestPath(mi*N+mj,ri*N+rj);
        System.out.println(path);
        if (!path.isEmpty()){
            int loc = path.get(0);
            move = new Site(loc/N,loc%N);
            System.out.println(move);
        }else
            move = rogue;

        return move;
    }

}
