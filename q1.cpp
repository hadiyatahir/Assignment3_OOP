//#if 0
#include <iostream>
#include "q1.h"
//#include <cstdlib>
using namespace std;

/////////////////INTERNAL SHAPE///////////////////////////


InternalShape::InternalShape(int w, int h) {
    width = w;
    height = h;
    grid = new bool* [width];
    for (int i = 0; i < width; i++) {

        grid[i] = new bool[height];
        for (int j = 0; j < height; j++)
            grid[i][j] = true;
    }
}

InternalShape::~InternalShape() {

    for (int i = 0; i < width; i++)
        delete[] grid[i];

    delete[] grid;
}

/* void resize(int newW, int newH) {
     for (int i = 0; i < width; i++)

         delete[] grid[i];
     delete[] grid;

     width = newW;
     height = newH;
     grid = new bool* [width];
     for (int i = 0; i < width; i++) {

         grid[i] = new bool[height];

         for (int j = 0; j < height; j++)
             grid[i][j] = true;
     }
 }*/


/////////////////////////ORGANISM///////////////////////

Organism::Organism() {
    x = 0;
    y = 0;
    shape = nullptr;
    alive = true;

    pchild = nullptr;
}

Organism::~Organism() {
    delete shape;
}

void Organism::resize(int newW, int newH) {
    delete shape;

    shape = new InternalShape(newW, newH);
}

float Organism::getAverageNutrients(Tile** world) {

    float sum = 0;
    int c = 0;
    for (int i = 0; i < shape->width; i++) {
        for (int j = 0; j < shape->height; j++) {

            if (!shape->grid[i][j])
                continue;

            int gx = x + i;
            int gy = y + j;

            if (gx < 0 || gx >= worldWidth || gy < 0 || gy >= worldHeight)

                continue;

            sum += world[gx][gy].nutrientLevel;
            c++;
        }
    }

    if (c > 0)
        return sum / c;
    else
        return 0.0f;
}

void Organism::update(Tile** world, int worldWidth, int worldHeight) {}

bool Organism::isSprawler() {

    return false;
}


//////////////////////FRACTAL SPRAWNLER////////////////////////////////////////

FractalSprawler::FractalSprawler() {
    resize(2, 2);
}

bool FractalSprawler::isSprawler() {
    return true;
}

void FractalSprawler::update(Tile** world, int worldWidth, int worldHeight) {

    //float avg = getAverageNutrients(world, worldWidth, worldHeight);

    int cx = x + shape->width / 2;
    int cy = y + shape->height / 2;

    int radius = 2 * shape->width;

    float sum = 0;
    int count = 0;
    for (int i = cx - radius; i <= cx + radius; i++) {
        for (int j = cy - radius; j <= cy + radius; j++) {

            if (i < 0 || j < 0 || i >= worldWidth || j >= worldHeight)

                continue;
            sum += world[i][j].nutrientLevel;

            count++;
        }
    }

    float avg;
    if (count > 0)
        avg = sum / count;
    else
        avg = 0.0f;

    if (avg > 70) {
        int nw = shape->width + 2;
        int nh = shape->height + 2;

        if (x + nw <= worldWidth && y + nh <= worldHeight)
            resize(nw, nh);

    }

    else if (avg < 30) {

        if (shape->width - 1 <= 0 || shape->height - 1 <= 0) {
            alive = false;
            return;
        }

        resize(shape->width - 1, shape->height - 1);
    }

    if (shape->width >= 5 && shape->height >= 5) {
        FractalSprawler* child = new FractalSprawler();

        child->worldWidth = worldWidth;
        child->worldHeight = worldHeight;

        child->x = x + shape->width + 1;

        child->y = y;

        if (child->x + child->shape->width > worldWidth)

            child->x = 0;

        bool empty = true;
        for (int i = 0; i < child->shape->width && empty; i++) {

            for (int j = 0; j < child->shape->height && empty; j++) {

                int gx = child->x + i;
                int gy = child->y + j;

                if (gx < 0 || gx >= worldWidth || gy < 0 || gy >= worldHeight || world[gx][gy].occupant != nullptr)

                    empty = false;
            }
        }

        if (empty) {
            pchild = child;
            resize(2, 2);

        }
        else {
            delete child;
        }

    }
}

/////////////////////KINETIC HUNTER//////////////////////////////////////////////////

KineticHunter::KineticHunter() {
    resize(2, 2);
}

void KineticHunter::update(Tile** world, int worldWidth, int worldHeight) {

    int radius = shape->width * 4;

    bool moved = false;
    int targetX = -1, targetY = -1;

    int maxval = worldWidth * worldWidth + worldHeight * worldHeight + 1;
    int mindist = maxval;

    for (int i = x - radius; i <= x + radius; i++) {

        for (int j = y - radius; j <= y + radius; j++) {
            if (i < 0 || j < 0 || i >= worldWidth || j >= worldHeight)
                continue;

            Organism* target = world[i][j].occupant;

            if (target == nullptr || !target->isSprawler())
                continue;

            int dist = (i - x) * (i - x) + (j - y) * (j - y);
            if (dist < mindist) {
                mindist = dist;
                targetX = i;

                targetY = j;
            }
        }

    }

    if (targetX != -1) {
        if (targetX > x)
            x++;
        else if (targetX < x)
            x--;
        if (targetY > y)
            y++;
        else if (targetY < y)
            y--;
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;
        if (x + shape->width > worldWidth)
            x = worldWidth - shape->width;
        if (y + shape->height > worldHeight)

            y = worldHeight - shape->height;

        moved = true;
    }


    if (!moved) {
        int dx = (rand() % 3) - 1;
        int dy = (rand() % 3) - 1;

        x += dx;

        y += dy;
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;

        if (x + shape->width > worldWidth)
            x = worldWidth - shape->width;

        if (y + shape->height > worldHeight)
            y = worldHeight - shape->height;
    }

    for (int dx = 0; dx < shape->width; dx++) {

        for (int dy = 0; dy < shape->height; dy++) {
            if (!shape->grid[dx][dy])
                continue;

            int gx = x + dx;
            int gy = y + dy;

            if (gx < 0 || gx >= worldWidth || gy < 0 || gy >= worldHeight)

                continue;

            Organism* target = world[gx][gy].occupant;

            if (target == nullptr || !target->isSprawler())

                continue;

            world[gx][gy].nutrientLevel -= 5.0f;

            if (target->shape->width - 1 <= 0 || target->shape->height - 1 <= 0)
                target->alive = false;
            else
                target->resize(target->shape->width - 1, target->shape->height - 1);
        }
    }
}


//////////////////////WORLD///////////////////////////

World::World(int w, int h, int m) {
    width = w;
    height = h;
    max = m;

    grid = new Tile * [width];
    for (int i = 0; i < width; i++) {
        grid[i] = new Tile[height];

        for (int j = 0; j < height; j++) {

            grid[i][j].nutrientLevel = static_cast<float>(rand() % 101);

            grid[i][j].toxicity = static_cast<float>(rand() % 101);

            grid[i][j].occupant = nullptr;
        }
    }

    population = new Organism * [max];
    for (int i = 0; i < max; i++)
        population[i] = nullptr;
}

World::~World() {
    for (int i = 0; i < max; i++)
        if (population[i])
            delete population[i];


    delete[] population;
    for (int i = 0; i < width; i++)

        delete[] grid[i];

    delete[] grid;
}

bool World::addOrganism(Organism* o) {
    for (int i = 0; i < max; i++) {
        if (population[i] == nullptr) {
            o->worldWidth = width;
            o->worldHeight = height;

            population[i] = o;
            return true;
        }
    }
    return false;
}

void World::updateTiles() {
    for (int i = 0; i < width; i++)

        for (int j = 0; j < height; j++)
            grid[i][j].occupant = nullptr;

    for (int i = 0; i < max; i++) {

        if (population[i] == nullptr || !population[i]->alive)

            continue;
        Organism* o = population[i];

        for (int dx = 0; dx < o->shape->width; dx++) {
            for (int dy = 0; dy < o->shape->height; dy++) {

                if (!o->shape->grid[dx][dy])
                    continue;

                int gx = o->x + dx;
                int gy = o->y + dy;

                //if (gx >= 0 && gx < width && gy >= 0 && gy < height)

                if (gx >= 0 && gx < width && gy >= 0 && gy < height) {
                    if (grid[gx][gy].occupant == nullptr || !o->isSprawler())
                        grid[gx][gy].occupant = o;
                }

                //grid[gx][gy].occupant = o;
            }


        }
    }
}

void World::display() {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (grid[i][j].occupant == nullptr)

                cout << "\033[40m  \033[0m";
            else if (grid[i][j].occupant->isSprawler())
                cout << "\033[42m  \033[0m";

            else
                cout << "\033[41m  \033[0m";
        }
        cout << "\n";
    }

    int alive = 0;
    for (int i = 0; i < max; i++)

        if (population[i] && population[i]->alive)

            alive++;
    float totaltox = 0;
    for (int i = 0; i < width; i++)

        for (int j = 0; j < height; j++)
            totaltox += grid[i][j].toxicity;

    cout << "Total Population: " << alive << "\n";

    cout << "Average World Toxicity: " << totaltox / (width * height) << "\n";
}

void World::runIteration() {

    for (int i = 0; i < max; i++)
        if (population[i] && population[i]->alive)

            population[i]->update(grid, width, height);

    for (int i = 0; i < max; i++) {
        if (population[i] == nullptr)
            continue;
        if (population[i]->pchild != nullptr) {

            if (!addOrganism(population[i]->pchild))

                delete population[i]->pchild;

            population[i]->pchild = nullptr;
        }
        if (!population[i]->alive) {

            delete population[i];

            population[i] = nullptr;
        }
    }

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)

            if (grid[i][j].toxicity > 0)
                grid[i][j].toxicity -= 0.5f;


    updateTiles();
    display();
}


///////////////////////////////////////////////MAIN/////////////////////////////////////////////

int main() {
    srand(42);

    int w, h, numS, numH, it;
    cout << "Enter world width: ";

    cin >> w;
    cout << "Enter world height: ";
    cin >> h;
    cout << "Enter number of Fractal Sprawlers: ";
    cin >> numS;
    cout << "Enter number of Kinetic Hunters: ";
    cin >> numH;
    cout << "Enter number of iterations: ";

    cin >> it;

    int max = (numS + numH) * 5;
    World world(w, h, max);

    for (int i = 0; i < numS; i++) {
        FractalSprawler* f = new FractalSprawler();
        f->x = rand() % (w - 2);

        f->y = rand() % (h - 2);
        world.addOrganism(f);
    }
    for (int i = 0; i < numH; i++) {
        KineticHunter* k = new KineticHunter();

        k->x = rand() % (w - 2);
        k->y = rand() % (h - 2);

        world.addOrganism(k);
    }

    world.updateTiles();
    world.display();

    for (int i = 0; i < it; i++)

        world.runIteration();

    return 0;
}
//#endif