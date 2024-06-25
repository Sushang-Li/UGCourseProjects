import java.util.List;

public class Rogue extends Role{
    private Game game;
    private Dungeon dungeon;
    private int N;
    private boolean hasCorridor;

    public Rogue(Game game) {
        this.game    = game;
        this.dungeon = game.getDungeon();
        this.N       = dungeon.size();
        this.hasCorridor = dungeon.haveCorridor();
    }

    //    move function for rogue
    @Override
    public Site move() {
        int mi = game.getMonsterSite().i();
        int mj = game.getMonsterSite().j();
        int ri = game.getRogueSite().i();
        int rj = game.getRogueSite().j();
        Site rogue   = game.getRogueSite();
        Site move    = null;

        List<Integer> path;
        // The rogue will move to the nearest corridor if the dungeon has a corridor
        if(hasCorridor){
            if(dungeon.isCorridor(ri, rj)){
                // The rogue will move to the nearest corridor if the rogue is in a corridor
                path = dungeon.findPathAwayFromMonster1(ri*N+rj,mi*N+mj);
            }else{
                // The rogue will move to the nearest corridor if the rogue is not in a corridor
                path = dungeon.findPathToNearestCorridor(ri*N+rj,mi*N+mj);
            }
        }else {
            // Default move
            path = dungeon.findPathAwayFromMonster1(ri*N+rj,mi*N+mj);
        }
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
