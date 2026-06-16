#pragma once


class Organism;

struct Tile {
    float nutrientLevel;
    float toxicity;
    Organism* occupant;
};

class InternalShape {
public:
    bool** grid;
    int width;

    int height;

    InternalShape(int w, int h);
    ~InternalShape();
};

class Organism {
public:
    int x, y;
    InternalShape* shape;
    int worldWidth;
    int worldHeight;
    bool alive;

    Organism* pchild;

    Organism();
    virtual ~Organism();

    void resize(int newW, int newH);
    float getAverageNutrients(Tile** world);

    virtual void update(Tile** world, int worldWidth, int worldHeight);

    virtual bool isSprawler();
};



class FractalSprawler : public Organism {
public:
    FractalSprawler();
    bool isSprawler() override;
    void update(Tile** world, int worldWidth, int worldHeight);
};



class KineticHunter : public Organism {
public:
    KineticHunter();

    void update(Tile** world, int worldWidth, int worldHeight);
};



class World {
public:
    Tile** grid;
    Organism** population;
    int width;
    int height;
    int max;

    World(int w, int h, int maxOrganisms);
    ~World();

    bool addOrganism(Organism* o);
    void updateTiles();
    void display();
    void runIteration();
};
