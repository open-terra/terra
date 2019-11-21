#pragma once

#include <vector>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "types/vec2.hpp"

#define DELETED -2
#define le 0
#define re 1

namespace terra
{
    /*
    * graph_edge
    * handle an edge of the Voronoi Diagram.
    */
    class graph_edge
    {
    public:
        /// coordinates of the extreme points
        double x1,y1,x2,y2;

        /// indices of the parent sites that define the edge
        int point1, point2;

        /// pointer to the next edge
        graph_edge* next;
    };

    /*
    * site
    * structure used both for particle sites and for vertices.
    */
    class site
    {
    public:
        terra::vec2	coord;
        int sitenbr;
        int refcnt;
    };



    class free_node
    {
    public:
        free_node *nextfree;
    };


    class free_node_array_list
    {
    public:
        free_node* memory;
        free_node_array_list* next;
    };


    class free_list
    {
    public:
        free_node *head;
        int nodesize;
    };

    class edge
    {
    public:
        double a,b,c;
        site *ep[2];
        site *reg[2];
        int edgenbr;
    };


    class half_edge
    {
    public:
        half_edge *ELleft, *ELright;
        edge *ELedge;
        int ELrefcnt;
        char ELpm;
        site *vertex;
        volatile double ystar;
        half_edge *PQnext;
    };

    /*
    * voronoi_diagram_generator
    * Shane O'Sullivan C++ version of Stephan Fortune Voronoi diagram
    * generator
    */
    class voronoi_diagram_generator
    {
    public:
        voronoi_diagram_generator();
        ~voronoi_diagram_generator();

        bool generate_voronoi(std::vector<terra::vec2> *_parent_sites,
            double minX, double maxX, double minY, double maxY,
            double minDist=0);

        inline void reset_iterator()
        {
            iteratoredges = alledges;
        }

        bool get_next(graph_edge **e)
        {
            if(iteratoredges == 0)
                return false;

            *e = iteratoredges;
            iteratoredges = iteratoredges->next;
            return true;
        }

        std::vector<terra::vec2> *parent_sites;
        int n_parent_sites;

    private:
        void cleanup();
        void cleanupedges();
        char *getfree(free_list *fl);
        half_edge *PQfind();
        int PQempty();

        half_edge **ELhash;
        half_edge *HEcreate(), *ELleft(), *ELright(), *ELleftbnd();
        half_edge *HEcreate(edge *e,int pm);

        terra::vec2 PQ_min();
        half_edge *PQextractmin();
        void freeinit(free_list *fl,int size);
        void makefree(free_node *curr,free_list *fl);
        void geominit();
        void plotinit();

        // GS: removed the unused (always ==0) argument
        bool voronoi(/*int triangulate*/);
        void ref(site *v);
        void deref(site *v);
        void endpoint(edge *e,int lr,site * s);

        void ELdelete(half_edge *he);
        half_edge *ELleftbnd(terra::vec2 *p);
        half_edge *ELright(half_edge *he);
        void makevertex(site *v);

        void PQinsert(half_edge *he,site * v, double offset);
        void PQdelete(half_edge *he);
        bool ELinitialize();
        void ELinsert(half_edge *lb, half_edge *newHe);
        half_edge * ELgethash(int b);
        half_edge *ELleft(half_edge *he);
        site *leftreg(half_edge *he);
        bool PQinitialize();
        int PQbucket(half_edge *he);
        void clip_line(edge *e);
        char *myalloc(unsigned n);
        int right_of(half_edge *el,terra::vec2 *p);

        site *rightreg(half_edge *he);
        edge *bisect(site *s1, site *s2);
        double dist(site *s,site *t);

        // GS: 'p' is unused and always ==0 (see also comment by
        //     S. O'Sullivan in the source file), so we remove it
        site *intersect(half_edge *el1, half_edge *el2 /*, terra::vec2 *p=0*/);

        site *nextone();

        void pushgraph_edge(double x1, double y1, double x2, double y2,
            site *s1, site *s2);

        // Gregory Soyez: unused plotting methods
        // void openpl();
        // void circle(double x, double y, double radius);
        // void range(double minX, double minY, double maxX, double maxY);
        //
        // void out_bisector(edge *e);
        // void out_ep(edge *e);
        // void out_vertex(site *v);
        // void out_site(site *s);
        //
        // void out_triple(site *s1, site *s2,site * s3);

        free_list hfl;
        half_edge *ELleftend, *ELrightend;
        int ELhashsize;

        int sorted, debug;
        double xmin, xmax, ymin, ymax, deltax, deltay;

        site *sites;
        int nsites;
        int siteidx;
        int sqrt_nsites;
        int nvertices;
        free_list sfl;
        site *bottomsite;

        int nedges;
        free_list efl;
        int PQhashsize;
        half_edge *PQhash;
        int PQcount;
        int PQmin;

        int ntry, totalsearch;
        double pxmin, pxmax, pymin, pymax, cradius;
        int total_alloc;

        double borderMinX, borderMaxX, borderMinY, borderMaxY;

        free_node_array_list* allMemoryList;
        free_node_array_list* currentMemoryBlock;

        graph_edge* alledges;
        graph_edge* iteratoredges;

        double minDistanceBetweensites;
    };

    int scomp(const void *p1,const void *p2);
}
