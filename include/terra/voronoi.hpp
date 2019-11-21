#pragma once

#include <vector>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "types/vec2.hpp"

#define DELETED -2
#define le 0
#define re 1

namespace Terra
{
    /*
    * GraphEdge
    * handle an edge of the Voronoi Diagram.
    */
    class GraphEdge
    {
    public:
        /// coordinates of the extreme points
        double x1,y1,x2,y2;

        /// indices of the parent sites that define the edge
        int point1, point2;

        /// pointer to the next edge
        GraphEdge* next;
    };

    /*
    * Site
    * structure used both for particle sites and for vertices.
    */
    class Site
    {
    public:
        Terra::vec2	coord;
        int sitenbr;
        int refcnt;
    };



    class Freenode
    {
    public:
        Freenode *nextfree;
    };


    class FreeNodeArrayList
    {
    public:
        Freenode* memory;
        FreeNodeArrayList* next;
    };


    class Freelist
    {
    public:
        Freenode *head;
        int nodesize;
    };

    class Edge
    {
    public:
        double a,b,c;
        Site *ep[2];
        Site *reg[2];
        int edgenbr;
    };


    class Halfedge
    {
    public:
        Halfedge *ELleft, *ELright;
        Edge *ELedge;
        int ELrefcnt;
        char ELpm;
        Site *vertex;
        volatile double ystar;
        Halfedge *PQnext;
    };

    /*
    * VoronoiDiagramGenerator
    * Shane O'Sullivan C++ version of Stephan Fortune Voronoi diagram
    * generator
    */
    class VoronoiDiagramGenerator
    {
    public:
        VoronoiDiagramGenerator();
        ~VoronoiDiagramGenerator();

        bool generateVoronoi(std::vector<Terra::vec2> *_parent_sites,
            double minX, double maxX, double minY, double maxY,
            double minDist=0);

        inline void resetIterator()
        {
            iteratorEdges = allEdges;
        }

        bool getNext(GraphEdge **e)
        {
            if(iteratorEdges == 0)
                return false;

            *e = iteratorEdges;
            iteratorEdges = iteratorEdges->next;
            return true;
        }

        std::vector<Terra::vec2> *parent_sites;
        int n_parent_sites;

    private:
        void cleanup();
        void cleanupEdges();
        char *getfree(Freelist *fl);
        Halfedge *PQfind();
        int PQempty();

        Halfedge **ELhash;
        Halfedge *HEcreate(), *ELleft(), *ELright(), *ELleftbnd();
        Halfedge *HEcreate(Edge *e,int pm);

        Terra::vec2 PQ_min();
        Halfedge *PQextractmin();
        void freeinit(Freelist *fl,int size);
        void makefree(Freenode *curr,Freelist *fl);
        void geominit();
        void plotinit();

        // GS: removed the unused (always ==0) argument
        bool voronoi(/*int triangulate*/);
        void ref(Site *v);
        void deref(Site *v);
        void endpoint(Edge *e,int lr,Site * s);

        void ELdelete(Halfedge *he);
        Halfedge *ELleftbnd(Terra::vec2 *p);
        Halfedge *ELright(Halfedge *he);
        void makevertex(Site *v);

        void PQinsert(Halfedge *he,Site * v, double offset);
        void PQdelete(Halfedge *he);
        bool ELinitialize();
        void ELinsert(Halfedge *lb, Halfedge *newHe);
        Halfedge * ELgethash(int b);
        Halfedge *ELleft(Halfedge *he);
        Site *leftreg(Halfedge *he);
        bool PQinitialize();
        int PQbucket(Halfedge *he);
        void clip_line(Edge *e);
        char *myalloc(unsigned n);
        int right_of(Halfedge *el,Terra::vec2 *p);

        Site *rightreg(Halfedge *he);
        Edge *bisect(Site *s1, Site *s2);
        double dist(Site *s,Site *t);

        // GS: 'p' is unused and always ==0 (see also comment by
        //     S. O'Sullivan in the source file), so we remove it
        Site *intersect(Halfedge *el1, Halfedge *el2 /*, Terra::vec2 *p=0*/);

        Site *nextone();

        void pushGraphEdge(double x1, double y1, double x2, double y2,
            Site *s1, Site *s2);

        // Gregory Soyez: unused plotting methods
        // void openpl();
        // void circle(double x, double y, double radius);
        // void range(double minX, double minY, double maxX, double maxY);
        //
        // void out_bisector(Edge *e);
        // void out_ep(Edge *e);
        // void out_vertex(Site *v);
        // void out_site(Site *s);
        //
        // void out_triple(Site *s1, Site *s2,Site * s3);

        Freelist hfl;
        Halfedge *ELleftend, *ELrightend;
        int ELhashsize;

        int sorted, debug;
        double xmin, xmax, ymin, ymax, deltax, deltay;

        Site *sites;
        int nsites;
        int siteidx;
        int sqrt_nsites;
        int nvertices;
        Freelist sfl;
        Site *bottomsite;

        int nedges;
        Freelist efl;
        int PQhashsize;
        Halfedge *PQhash;
        int PQcount;
        int PQmin;

        int ntry, totalsearch;
        double pxmin, pxmax, pymin, pymax, cradius;
        int total_alloc;

        double borderMinX, borderMaxX, borderMinY, borderMaxY;

        FreeNodeArrayList* allMemoryList;
        FreeNodeArrayList* currentMemoryBlock;

        GraphEdge* allEdges;
        GraphEdge* iteratorEdges;

        double minDistanceBetweenSites;
    };

    int scomp(const void *p1,const void *p2);
}
