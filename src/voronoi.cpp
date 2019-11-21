#include "terra/voronoi.hpp"

#include <stdio.h>

using namespace terra;

voronoi_diagram_generator::voronoi_diagram_generator()
{
    siteidx = 0;
    sites = NULL;
    parent_sites = NULL;

    allMemoryList = new free_node_array_list;
    allMemoryList->memory = NULL;
    allMemoryList->next = NULL;
    currentMemoryBlock = allMemoryList;
    alledges = NULL;
    iteratoredges = 0;
    minDistanceBetweensites = 0;

    ELhash = NULL;
    PQhash = NULL;
}

voronoi_diagram_generator::~voronoi_diagram_generator()
{
    cleanup();
    cleanupedges();

    if (allMemoryList != NULL)
        delete allMemoryList;
}

bool voronoi_diagram_generator::generate_voronoi(
    std::vector<terra::vec2>* _parent_sites,
    double minX,
    double maxX,
    double minY,
    double maxY,
    double minDist)
{
    cleanup();
    cleanupedges();
    int i;
    double x, y;

    minDistanceBetweensites = minDist;

    parent_sites = _parent_sites;

    nsites = n_parent_sites = parent_sites->size();
    debug = 1;
    sorted = 0;
    freeinit(&sfl, sizeof(site));

    // sites = (site *) myalloc(nsites*sizeof( *sites));
    sites = (site*)myalloc(nsites * sizeof(site));
    // parent_sites = (site *) myalloc(nsites*sizeof( site));

    if (sites == 0)
        return false;

    xmax = xmin = (*parent_sites)[0].x;
    ymax = ymin = (*parent_sites)[0].y;

    for (i = 0; i < nsites; i++)
    {
        x = (*parent_sites)[i].x;
        y = (*parent_sites)[i].y;
        sites[i].coord.x = x;
        sites[i].coord.y = y;
        sites[i].sitenbr = i;
        sites[i].refcnt = 0;

        if (x < xmin)
            xmin = x;
        else if (x > xmax)
            xmax = x;

        if (y < ymin)
            ymin = y;
        else if (y > ymax)
            ymax = y;
    }

    qsort(sites, nsites, sizeof(*sites), scomp);

    // Gregory Soyez
    //
    // check if some of the particles are degenerate to avoid a crash.
    //
    // at the moment, we work under the assumption that they will be
    // "clustered" later on, so we just keep the 1st one and discard the
    // others
    unsigned int offset = 0;
    for (int is = 1; is < nsites; is++)
    {
        if (sites[is].coord.y == sites[is - 1].coord.y &&
            sites[is].coord.x == sites[is - 1].coord.x)
        {
            offset++;
        }
        else if (offset > 0)
        {
            sites[is - offset] = sites[is];
        }
    }

    if (offset > 0)
    {
        nsites -= offset;
        //_warning_degeneracy.warn("voronoi_diagram_generator: two (or more)
        //particles are degenerate in rapidity and azimuth, Voronoi cell
        //assigned to the first of each set of degenerate particles.");
    }

    siteidx = 0;
    geominit();
    double temp = 0;
    if (minX > maxX)
    {
        temp = minX;
        minX = maxX;
        maxX = temp;
    }
    if (minY > maxY)
    {
        temp = minY;
        minY = maxY;
        maxY = temp;
    }
    borderMinX = minX;
    borderMinY = minY;
    borderMaxX = maxX;
    borderMaxY = maxY;

    siteidx = 0;
    voronoi();

    return true;
}

bool voronoi_diagram_generator::ELinitialize()
{
    int i;
    freeinit(&hfl, sizeof(half_edge));
    ELhashsize = 2 * sqrt_nsites;
    // ELhash = (half_edge **) myalloc ( sizeof *ELhash * ELhashsize);
    ELhash = (half_edge**)myalloc(sizeof(half_edge*) * ELhashsize);

    if (ELhash == 0)
        return false;

    for (i = 0; i < ELhashsize; i += 1)
        ELhash[i] = (half_edge*)NULL;
    ELleftend = HEcreate((edge*)NULL, 0);
    ELrightend = HEcreate((edge*)NULL, 0);
    ELleftend->ELleft = (half_edge*)NULL;
    ELleftend->ELright = ELrightend;
    ELrightend->ELleft = ELleftend;
    ELrightend->ELright = (half_edge*)NULL;
    ELhash[0] = ELleftend;
    ELhash[ELhashsize - 1] = ELrightend;

    return true;
}

half_edge* voronoi_diagram_generator::HEcreate(edge* e, int pm)
{
    half_edge* answer;
    answer = (half_edge*)getfree(&hfl);
    answer->ELedge = e;
    answer->ELpm = pm;
    answer->PQnext = (half_edge*)NULL;
    answer->vertex = (site*)NULL;
    answer->ELrefcnt = 0;
    return answer;
}

void voronoi_diagram_generator::ELinsert(half_edge* lb, half_edge* newHe)
{
    newHe->ELleft = lb;
    newHe->ELright = lb->ELright;
    (lb->ELright)->ELleft = newHe;
    lb->ELright = newHe;
}

/* Get entry from hash table, pruning any deleted nodes */
half_edge* voronoi_diagram_generator::ELgethash(int b)
{
    half_edge* he;

    if ((b < 0) || (b >= ELhashsize))
        return (half_edge*)NULL;

    he = ELhash[b];
    if ((he == (half_edge*)NULL) || (he->ELedge != (edge*)DELETED))
        return he;

    /* Hash table points to deleted half edge.  Patch as necessary. */
    ELhash[b] = (half_edge*)NULL;
    if ((he->ELrefcnt -= 1) == 0)
        makefree((free_node*)he, &hfl);
    return (half_edge*)NULL;
}

half_edge* voronoi_diagram_generator::ELleftbnd(terra::vec2* p)
{
    int i, bucket;
    half_edge* he;

    /* Use hash table to get close to desired halfedge */
    // use the hash function to find the place in the hash map that this
    // half_edge should be
    // Gregory Soyez: the original code was
    //
    //   bucket = (int)((p->x - xmin)/deltax * ELhashsize);
    //   // make sure that the bucket position in within the range of the
    //   //hash array
    //   if (bucket<0) bucket =0;
    //   if (bucket>=ELhashsize) bucket = ELhashsize - 1;
    //
    // but this runs the risk of having a overflow which would
    // cause bucket to be truncated at 0 instead of ELhashsize - 1
    // (or vice-versa)
    // We fix this by performing the test immediately on the double
    // We put in a extra bit of margin to be sure the conversion does
    // not play dirty tricks on us

    // const double &px = p->x;
    if (p->x < xmin)
    {
        bucket = 0;
    }
    else if (p->x >= xmax)
    {
        bucket = ELhashsize - 1;
    }
    else
    {
        bucket = (int)((p->x - xmin) / deltax * ELhashsize);
        if (bucket >= ELhashsize)
            bucket = ELhashsize - 1; // the lower cut should be robust
    }

    he = ELgethash(bucket);

    // if the HE isn't found, search backwards and forwards in the hash
    // map for the first non-null entry
    if (he == (half_edge*)NULL)
    {
        for (i = 1; 1; i++)
        {
            if ((he = ELgethash(bucket - i)) != (half_edge*)NULL)
                break;
            if ((he = ELgethash(bucket + i)) != (half_edge*)NULL)
                break;
        };
        totalsearch += i;
    };
    ntry += 1;
    /* Now search linear list of halfedges for the correct one */
    if ((he == ELleftend) || (he != ELrightend && right_of(he, p)))
    {
        do
        {
            he = he->ELright;
        } while (he != ELrightend && right_of(he, p));
        // keep going right on the list until either the end is reached,
        // or you find the 1st edge which the point
        he = he->ELleft; // isn't to the right of
    }
    else
        // if the point is to the left of the half_edge, then search left
        // for the HE just to the left of the point
        do
        {
            he = he->ELleft;
        } while ((he != ELleftend) && (!right_of(he, p)));

    /* Update hash table and reference counts */
    if ((bucket > 0) && (bucket < ELhashsize - 1))
    {
        if (ELhash[bucket] != (half_edge*)NULL)
        {
            ELhash[bucket]->ELrefcnt -= 1;
        }
        ELhash[bucket] = he;
        ELhash[bucket]->ELrefcnt += 1;
    };

    return he;
}

/* This delete routine can't reclaim node, since pointers from hash
   table may be present.   */
void voronoi_diagram_generator::ELdelete(half_edge* he)
{
    (he->ELleft)->ELright = he->ELright;
    (he->ELright)->ELleft = he->ELleft;
    he->ELedge = (edge*)DELETED;
}

half_edge* voronoi_diagram_generator::ELright(half_edge* he)
{
    return he->ELright;
}

half_edge* voronoi_diagram_generator::ELleft(half_edge* he)
{
    return he->ELleft;
}

site* voronoi_diagram_generator::leftreg(half_edge* he)
{
    if (he->ELedge == (edge*)NULL)
        return (bottomsite);
    return (he->ELpm == le) ? he->ELedge->reg[le] : he->ELedge->reg[re];
}

site* voronoi_diagram_generator::rightreg(half_edge* he)
{
    if (he->ELedge == (edge*)NULL)
        // if this halfedge has no edge, return the bottom site (whatever
        // that is)
        return bottomsite;

    // if the ELpm field is zero, return the site 0 that this edge
    // bisects, otherwise return site number 1
    return (he->ELpm == le) ? he->ELedge->reg[re] : he->ELedge->reg[le];
}

void voronoi_diagram_generator::geominit()
{
    double sn;

    freeinit(&efl, sizeof(edge));
    nvertices = 0;
    nedges = 0;
    sn = (double)nsites + 4;
    sqrt_nsites = (int)sqrt(sn);
    deltay = ymax - ymin;
    deltax = xmax - xmin;
}

edge* voronoi_diagram_generator::bisect(site* s1, site* s2)
{
    double dx, dy, adx, ady;
    edge* newedge;

    newedge = (edge*)getfree(&efl);

    newedge->reg[0] = s1; // store the sites that this edge is bisecting
    newedge->reg[1] = s2;
    ref(s1);
    ref(s2);

    // to begin with, there are no endpoints on the bisector - it goes
    // to infinity
    newedge->ep[0] = (site*)NULL;
    newedge->ep[1] = (site*)NULL;

    // get the difference in x dist between the sites
    dx = s2->coord.x - s1->coord.x;
    dy = s2->coord.y - s1->coord.y;

    // make sure that the difference is positive
    adx = dx > 0 ? dx : -dx;
    ady = dy > 0 ? dy : -dy;

    // get the slope of the line
    newedge->c = (double)(s1->coord.x * dx + s1->coord.y * dy +
                          (dx * dx + dy * dy) * 0.5);

    if (adx > ady)
    {
        // set formula of line, with x fixed to 1
        newedge->a = 1.0;
        newedge->b = dy / dx;
        newedge->c /= dx;
    }
    else
    {
        // set formula of line, with y fixed to 1
        newedge->b = 1.0;
        newedge->a = dx / dy;
        newedge->c /= dy;
    }

    newedge->edgenbr = nedges;
    nedges++;

    return newedge;
}

// create a new site where the half_edges el1 and el2 intersect - note
// that the terra::vec2 in the argument list is not used, don't know why
// it's there
//
// Gregory Soyez: removed the uinused point p
site* voronoi_diagram_generator::intersect(half_edge* el1, half_edge* el2
                                           /*, terra::vec2 *p*/)
{
    edge *e1, *e2, *e;
    half_edge* el;
    double d, xint, yint;
    int right_of_site;
    site* v;

    e1 = el1->ELedge;
    e2 = el2->ELedge;
    if ((e1 == (edge*)NULL) || (e2 == (edge*)NULL))
        return ((site*)NULL);

    // if the two edges bisect the same parent, return null
    if (e1->reg[1] == e2->reg[1])
        return (site*)NULL;

    // Gregory Soyez:
    //
    // if the 2 parents are too close, the intersection is going to be
    // computed from the "long edges" of the triangle which could causes
    // large rounding errors. In this case, use the bisector of the 2
    // parents to find the interaction point
    //
    // The following replaces
    //   d = e1->a * e2->b - e1->b * e2->a;
    //   if (-1.0e-10<d && d<1.0e-10)
    //     return (site*) NULL;
    //
    //   xint = (e1->c*e2->b - e2->c*e1->b)/d;
    //   yint = (e2->c*e1->a - e1->c*e2->a)/d;

    double dx = e2->reg[1]->coord.x - e1->reg[1]->coord.x;
    double dy = e2->reg[1]->coord.y - e1->reg[1]->coord.y;
    double dxref = e1->reg[1]->coord.x - e1->reg[0]->coord.x;
    double dyref = e1->reg[1]->coord.y - e1->reg[0]->coord.y;

    if (dx * dx + dy * dy < 1e-14 * (dxref * dxref + dyref * dyref))
    {
        // make sure that the difference is positive
        double adx = dx > 0 ? dx : -dx;
        double ady = dy > 0 ? dy : -dy;

        // get the slope of the line
        double a, b;
        double c =
            (double)(e1->reg[1]->coord.x * dx + e1->reg[1]->coord.y * dy +
                     (dx * dx + dy * dy) * 0.5);

        if (adx > ady)
        {
            a = 1.0;
            b = dy / dx;
            c /= dx;
        }
        else
        {
            b = 1.0;
            a = dx / dy;
            c /= dy;
        }

        d = e1->a * b - e1->b * a;
        if (-1.0e-10 < d && d < 1.0e-10)
        {
            return (site*)NULL;
        }

        xint = (e1->c * b - c * e1->b) / d;
        yint = (c * e1->a - e1->c * a) / d;
    }
    else
    {
        d = e1->a * e2->b - e1->b * e2->a;
        if (-1.0e-10 < d && d < 1.0e-10)
        {
            return (site*)NULL;
        }

        xint = (e1->c * e2->b - e2->c * e1->b) / d;
        yint = (e2->c * e1->a - e1->c * e2->a) / d;
    }
    // end of Gregory Soyez's modifications

    volatile double local_y1 = e1->reg[1]->coord.y;
    volatile double local_y2 = e2->reg[1]->coord.y;

    if ((local_y1 < local_y2) ||
        ((local_y1 == local_y2) && (e1->reg[1]->coord.x < e2->reg[1]->coord.x)))
    {
        el = el1;
        e = e1;
    }
    else
    {
        el = el2;
        e = e2;
    }

    right_of_site = xint >= e->reg[1]->coord.x;
    if ((right_of_site && el->ELpm == le) || (!right_of_site && el->ELpm == re))
        return (site*)NULL;

    // create a new site at the point of intersection - this is a new
    // vector event waiting to happen
    v = (site*)getfree(&sfl);
    v->refcnt = 0;
    v->coord.x = xint;
    v->coord.y = yint;
    return v;
}

// HERE

/* returns 1 if p is to right of halfedge e */
int voronoi_diagram_generator::right_of(half_edge* el, terra::vec2* p)
{
    edge* e;
    site* topsite;
    int right_of_site, above, fast;
    double dxp, dyp, dxs, t1, t2, t3, yl;

    e = el->ELedge;
    topsite = e->reg[1];
    right_of_site = p->x > topsite->coord.x;
    if (right_of_site && el->ELpm == le)
        return (1);
    if (!right_of_site && el->ELpm == re)
        return (0);

    if (e->a == 1.0)
    {
        dyp = p->y - topsite->coord.y;
        dxp = p->x - topsite->coord.x;
        fast = 0;
        if (((!right_of_site) & (e->b < 0.0)) | (right_of_site & (e->b >= 0.0)))
        {
            above = dyp >= e->b * dxp;
            fast = above;
        }
        else
        {
            above = p->x + p->y * e->b > e->c;
            if (e->b < 0.0)
                above = !above;
            if (!above)
                fast = 1;
        };
        if (!fast)
        {
            dxs = topsite->coord.x - (e->reg[0])->coord.x;
            above = e->b * (dxp * dxp - dyp * dyp) <
                    dxs * dyp * (1.0 + 2.0 * dxp / dxs + e->b * e->b);
            if (e->b < 0.0)
                above = !above;
        };
    }
    else /*e->b==1.0 */
    {
        yl = e->c - e->a * p->x;
        t1 = p->y - yl;
        t2 = p->x - topsite->coord.x;
        t3 = yl - topsite->coord.y;
        above = t1 * t1 > t2 * t2 + t3 * t3;
    };
    return (el->ELpm == le ? above : !above);
}

void voronoi_diagram_generator::endpoint(edge* e, int lr, site* s)
{
    e->ep[lr] = s;
    ref(s);
    if (e->ep[re - lr] == (site*)NULL)
        return;

    clip_line(e);

    deref(e->reg[le]);
    deref(e->reg[re]);
    makefree((free_node*)e, &efl);
}

double voronoi_diagram_generator::dist(site* s, site* t)
{
    double dx, dy;
    dx = s->coord.x - t->coord.x;
    dy = s->coord.y - t->coord.y;
    return (double)(sqrt(dx * dx + dy * dy));
}

void voronoi_diagram_generator::makevertex(site* v)
{
    v->sitenbr = nvertices;
    nvertices += 1;
    // GS unused plot: out_vertex(v);
}

void voronoi_diagram_generator::deref(site* v)
{
    v->refcnt -= 1;
    if (v->refcnt == 0)
        makefree((free_node*)v, &sfl);
}

void voronoi_diagram_generator::ref(site* v)
{
    v->refcnt += 1;
}

// push the half_edge into the ordered linked list of vertices
void voronoi_diagram_generator::PQinsert(half_edge* he, site* v, double offset)
{
    half_edge *last, *next;

    he->vertex = v;
    ref(v);
    he->ystar = (double)(v->coord.y + offset);
    last = &PQhash[PQbucket(he)];
    while ((next = last->PQnext) != (half_edge*)NULL &&
           (he->ystar > next->ystar ||
            (he->ystar == next->ystar && v->coord.x > next->vertex->coord.x)))
    {
        last = next;
    };
    he->PQnext = last->PQnext;
    last->PQnext = he;
    PQcount += 1;
}

// remove the half_edge from the list of vertices
void voronoi_diagram_generator::PQdelete(half_edge* he)
{
    half_edge* last;

    if (he->vertex != (site*)NULL)
    {
        last = &PQhash[PQbucket(he)];
        while (last->PQnext != he)
            last = last->PQnext;

        last->PQnext = he->PQnext;
        PQcount -= 1;
        deref(he->vertex);
        he->vertex = (site*)NULL;
    };
}

int voronoi_diagram_generator::PQbucket(half_edge* he)
{
    // Gregory Soyez: the original code was
    //
    //   bucket = (int)((he->ystar - ymin)/deltay * PQhashsize);
    //   if (bucket<0) bucket = 0;
    //   if (bucket>=PQhashsize) bucket = PQhashsize-1 ;
    //   if (bucket < PQmin) PQmin = bucket;
    //   return(bucket);
    //
    // but this runs the risk of having a overflow which would
    // cause bucket to be truncated at 0 instead of PQhashsize-1
    // (or vice-versa)
    // We fix this by performing the test immediately on the double
    // We put in a extra bit of margin to be sure the conversion does
    // not play dirty tricks on us

    int bucket;

    double hey = he->ystar;
    if (hey < ymin)
    {
        bucket = 0;
    }
    else if (hey >= ymax)
    {
        bucket = PQhashsize - 1;
    }
    else
    {
        bucket = (int)((hey - ymin) / deltay * PQhashsize);
        if (bucket >= PQhashsize)
            bucket = PQhashsize - 1;
    }

    if (bucket < PQmin)
        PQmin = bucket;
    return (bucket);
}

int voronoi_diagram_generator::PQempty()
{
    return (PQcount == 0);
}

terra::vec2 voronoi_diagram_generator::PQ_min()
{
    terra::vec2 answer;

    while (PQhash[PQmin].PQnext == (half_edge*)NULL)
    {
        PQmin += 1;
    };
    answer.x = PQhash[PQmin].PQnext->vertex->coord.x;
    answer.y = PQhash[PQmin].PQnext->ystar;
    return (answer);
}

half_edge* voronoi_diagram_generator::PQextractmin()
{
    half_edge* curr;

    curr = PQhash[PQmin].PQnext;
    PQhash[PQmin].PQnext = curr->PQnext;
    PQcount -= 1;
    return (curr);
}

bool voronoi_diagram_generator::PQinitialize()
{
    int i;

    PQcount = 0;
    PQmin = 0;
    PQhashsize = 4 * sqrt_nsites;
    // PQhash = (half_edge *) myalloc(PQhashsize * sizeof *PQhash);
    PQhash = (half_edge*)myalloc(PQhashsize * sizeof(half_edge));

    if (PQhash == 0)
        return false;

    for (i = 0; i < PQhashsize; i += 1)
        PQhash[i].PQnext = (half_edge*)NULL;

    return true;
}

void voronoi_diagram_generator::freeinit(free_list* fl, int size)
{
    fl->head = (free_node*)NULL;
    fl->nodesize = size;
}

char* voronoi_diagram_generator::getfree(free_list* fl)
{
    int i;
    free_node* t;

    if (fl->head == (free_node*)NULL)
    {
        t = (free_node*)myalloc(sqrt_nsites * fl->nodesize);

        if (t == 0)
            return 0;

        currentMemoryBlock->next = new free_node_array_list;
        currentMemoryBlock = currentMemoryBlock->next;
        currentMemoryBlock->memory = t;
        currentMemoryBlock->next = 0;

        for (i = 0; i < sqrt_nsites; i += 1)
            makefree((free_node*)((char*)t + i * fl->nodesize), fl);
    };
    t = fl->head;
    fl->head = (fl->head)->nextfree;
    return ((char*)t);
}

void voronoi_diagram_generator::makefree(free_node* curr, free_list* fl)
{
    curr->nextfree = fl->head;
    fl->head = curr;
}

void voronoi_diagram_generator::cleanup()
{
    if (sites != NULL)
    {
        free(sites);
        sites = 0;
    }

    free_node_array_list *current = NULL, *prev = NULL;

    current = prev = allMemoryList;

    while (current->next != NULL)
    {
        prev = current;
        current = current->next;
        free(prev->memory);
        delete prev;
        prev = NULL;
    }

    if (current != NULL)
    {
        if (current->memory != NULL)
        {
            free(current->memory);
        }
        delete current;
    }

    allMemoryList = new free_node_array_list;
    allMemoryList->next = NULL;
    allMemoryList->memory = NULL;
    currentMemoryBlock = allMemoryList;

    if (ELhash != NULL)
        free(ELhash);

    if (PQhash != NULL)
        free(PQhash);
}

void voronoi_diagram_generator::cleanupedges()
{
    graph_edge *geCurrent = NULL, *gePrev = NULL;
    geCurrent = gePrev = alledges;

    while (geCurrent != NULL)
    {
        gePrev = geCurrent;
        geCurrent = geCurrent->next;
        delete gePrev;
    }

    alledges = 0;
}

void voronoi_diagram_generator::pushgraph_edge(double x1,
                                               double y1,
                                               double x2,
                                               double y2,
                                               site* s1,
                                               site* s2)
{
    graph_edge* newedge = new graph_edge;
    newedge->next = alledges;
    alledges = newedge;
    newedge->x1 = x1;
    newedge->y1 = y1;
    newedge->x2 = x2;
    newedge->y2 = y2;

    newedge->point1 = s1->sitenbr;
    newedge->point2 = s2->sitenbr;
}

char* voronoi_diagram_generator::myalloc(unsigned n)
{
    char* t = 0;
    t = (char*)malloc(n);
    total_alloc += n;
    return (t);
}

// unused plot functions
//
// /* for those who don't have Cherry's plot */
// /* #include <plot.h> */
// void voronoi_diagram_generator::openpl(){}
// void voronoi_diagram_generator::circle(double x, double y, double radius){}
// void voronoi_diagram_generator::range(double minX, double minY, double maxX,
// double maxY){}
//
//
//
// void voronoi_diagram_generator::out_bisector(edge *e)
// {
//
//
// }
//
//
// void voronoi_diagram_generator::out_ep(edge *e)
// {
//
//
// }
//
// void voronoi_diagram_generator::out_vertex(site *v)
// {
//
// }
//
//
// void voronoi_diagram_generator::out_site(site *s)
// {
//   // Gregory Soyez:
//   //   plot was always 0 so the expression below was always false
//   //   and even if it was not, 'circle' does nothing!
//   //
//   // if(!triangulate & plot & !debug)
//   //   circle (s->coord.x, s->coord.y, cradius);
//
// }
//
//
// void voronoi_diagram_generator::out_triple(site *s1, site *s2,site * s3)
// {
//
// }

void voronoi_diagram_generator::plotinit()
{
    double dx, dy, d;

    dy = ymax - ymin;
    dx = xmax - xmin;
    d = (double)((dx > dy ? dx : dy) * 1.1);
    pxmin = (double)(xmin - (d - dx) / 2.0);
    pxmax = (double)(xmax + (d - dx) / 2.0);
    pymin = (double)(ymin - (d - dy) / 2.0);
    pymax = (double)(ymax + (d - dy) / 2.0);
    cradius = (double)((pxmax - pxmin) / 350.0);
    // GS unused: openpl();
    // GS unused: range(pxmin, pymin, pxmax, pymax);
}

void voronoi_diagram_generator::clip_line(edge* e)
{
    site *s1, *s2;
    double x1 = 0, x2 = 0, y1 = 0, y2 = 0; //, temp = 0;

    x1 = e->reg[0]->coord.x;
    x2 = e->reg[1]->coord.x;
    y1 = e->reg[0]->coord.y;
    y2 = e->reg[1]->coord.y;

    // if the distance between the two points this line was created from is less
    // than the square root of 2, then ignore it
    // TODO improve/remove
    // if(sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))) <
    // minDistanceBetweensites)
    //  {
    //    return;
    //  }
    pxmin = borderMinX;
    pxmax = borderMaxX;
    pymin = borderMinY;
    pymax = borderMaxY;

    if (e->a == 1.0 && e->b >= 0.0)
    {
        s1 = e->ep[1];
        s2 = e->ep[0];
    }
    else
    {
        s1 = e->ep[0];
        s2 = e->ep[1];
    };

    if (e->a == 1.0)
    {
        y1 = pymin;
        if (s1 != (site*)NULL && s1->coord.y > pymin)
        {
            y1 = s1->coord.y;
        }
        if (y1 > pymax)
        {
            //	printf("\nClipped (1) y1 = %f to %f",y1,pymax);
            y1 = pymax;
            // return;
        }
        x1 = e->c - e->b * y1;
        y2 = pymax;
        if (s2 != (site*)NULL && s2->coord.y < pymax)
            y2 = s2->coord.y;

        if (y2 < pymin)
        {
            // printf("\nClipped (2) y2 = %f to %f",y2,pymin);
            y2 = pymin;
            // return;
        }
        x2 = (e->c) - (e->b) * y2;
        if (((x1 > pxmax) & (x2 > pxmax)) | ((x1 < pxmin) & (x2 < pxmin)))
        {
            // printf("\nClipLine jumping out(3), x1 = %f, pxmin = %f, pxmax =
            // %f",x1,pxmin,pxmax);
            return;
        }
        if (x1 > pxmax)
        {
            x1 = pxmax;
            y1 = (e->c - x1) / e->b;
        };
        if (x1 < pxmin)
        {
            x1 = pxmin;
            y1 = (e->c - x1) / e->b;
        };
        if (x2 > pxmax)
        {
            x2 = pxmax;
            y2 = (e->c - x2) / e->b;
        };
        if (x2 < pxmin)
        {
            x2 = pxmin;
            y2 = (e->c - x2) / e->b;
        };
    }
    else
    {
        x1 = pxmin;
        if (s1 != (site*)NULL && s1->coord.x > pxmin)
            x1 = s1->coord.x;
        if (x1 > pxmax)
        {
            // printf("\nClipped (3) x1 = %f to %f",x1,pxmin);
            // return;
            x1 = pxmax;
        }
        y1 = e->c - e->a * x1;
        x2 = pxmax;
        if (s2 != (site*)NULL && s2->coord.x < pxmax)
            x2 = s2->coord.x;
        if (x2 < pxmin)
        {
            // printf("\nClipped (4) x2 = %f to %f",x2,pxmin);
            // return;
            x2 = pxmin;
        }
        y2 = e->c - e->a * x2;
        if (((y1 > pymax) & (y2 > pymax)) | ((y1 < pymin) & (y2 < pymin)))
        {
            // printf("\nClipLine jumping out(6), y1 = %f, pymin = %f, pymax =
            // %f",y2,pymin,pymax);
            return;
        }
        if (y1 > pymax)
        {
            y1 = pymax;
            x1 = (e->c - y1) / e->a;
        };
        if (y1 < pymin)
        {
            y1 = pymin;
            x1 = (e->c - y1) / e->a;
        };
        if (y2 > pymax)
        {
            y2 = pymax;
            x2 = (e->c - y2) / e->a;
        };
        if (y2 < pymin)
        {
            y2 = pymin;
            x2 = (e->c - y2) / e->a;
        };
    };

    // printf("\nPushing line (%f,%f,%f,%f)",x1,y1,x2,y2);
    // fprintf(stdout, "Line with vertices (%f,%f) and (%f,%f)\n",
    //	e->reg[0]->coord.x, e->reg[1]->coord.x, e->reg[0]->coord.y,
    //e->reg[1]->coord.y);
    pushgraph_edge(x1, y1, x2, y2, e->reg[0], e->reg[1]);
}

/* implicit parameters: nsites, sqrt_nsites, xmin, xmax, ymin, ymax,
   deltax, deltay (can all be estimates).
   Performance suffers if they are wrong; better to make nsites,
   deltax, and deltay too big than too small.  (?) */

bool voronoi_diagram_generator::voronoi()
{
    site *newsite, *bot, *top, *temp, *p;
    site* v;
    terra::vec2 newintstar;
    int pm;
    half_edge *lbnd, *rbnd, *llbnd, *rrbnd, *bisector;
    edge* e;

    PQinitialize();
    bottomsite = nextone();
    // GS unused plot: out_site(bottomsite);
    bool retval = ELinitialize();

    if (!retval)
        return false;

    newsite = nextone();
    while (1)
    {

        if (!PQempty())
            newintstar = PQ_min();

        // if the lowest site has a smaller y value than the lowest vector
        // intersection, process the site otherwise process the vector
        // intersection
        if (newsite != (site*)NULL &&
            (PQempty() || newsite->coord.y < newintstar.y ||
             (newsite->coord.y == newintstar.y &&
              newsite->coord.x < newintstar.x)))
        { /* new site is smallest - this is a site event*/
            // GS unused plot: out_site(newsite);						//output the
            // site
            lbnd = ELleftbnd(&(newsite->coord)); // get the first half_edge to
                                                 // the LEFT of the new site
            rbnd = ELright(
                lbnd); // get the first half_edge to the RIGHT of the new site
            bot = rightreg(lbnd); // if this halfedge has no edge, , bot =
                                  // bottom site (whatever that is)
            e = bisect(bot, newsite); // create a new edge that bisects
            bisector = HEcreate(
                e, le); // create a new half_edge, setting its ELpm field to 0
            ELinsert(lbnd,
                     bisector); // insert this new bisector edge between the
                                // left and right vectors in a linked list

            if ((p = intersect(lbnd, bisector)) !=
                (site*)NULL) // if the new bisector intersects with the left
                             // edge, remove the left edge's vertex, and put in
                             // the new one
            {
                PQdelete(lbnd);
                PQinsert(lbnd, p, dist(p, newsite));
            };
            lbnd = bisector;
            bisector = HEcreate(
                e, re); // create a new half_edge, setting its ELpm field to 1
            ELinsert(lbnd, bisector); // insert the new HE to the right of the
                                      // original bisector earlier in the IF stmt

            if ((p = intersect(bisector, rbnd)) !=
                (site*)NULL) // if this new bisector intersects with the
            {
                PQinsert(bisector,
                         p,
                         dist(p, newsite)); // push the HE into the ordered
                                            // linked list of vertices
            };
            newsite = nextone();
        }
        else if (!PQempty()) /* intersection is smallest - this is a vector
                                event */
        {
            lbnd = PQextractmin(); // pop the half_edge with the lowest vector
                                   // off the ordered list of vectors
            llbnd =
                ELleft(lbnd); // get the half_edge to the left of the above HE
            rbnd =
                ELright(lbnd); // get the half_edge to the right of the above HE
            rrbnd = ELright(rbnd); // get the half_edge to the right of the HE
                                   // to the right of the lowest HE
            bot = leftreg(lbnd); // get the site to the left of the left HE
                                 // which it bisects
            top = rightreg(rbnd); // get the site to the right of the right HE
                                  // which it bisects

            // GS unused plot: out_triple(bot, top, rightreg(lbnd));		//output
            // the triple of sites, stating that a circle goes through them

            v = lbnd->vertex; // get the vertex that caused this event
            makevertex(v); // set the vertex number - couldn't do this earlier
                           // since we didn't know when it would be processed
            endpoint(
                lbnd->ELedge,
                lbnd->ELpm,
                v); // set the endpoint of the left half_edge to be this vector
            endpoint(
                rbnd->ELedge,
                rbnd->ELpm,
                v); // set the endpoint of the right half_edge to be this vector
            ELdelete(
                lbnd); // mark the lowest HE for deletion - can't delete yet
                       // because there might be pointers to it in Hash Map
            PQdelete(rbnd); // remove all vertex events to do with the  right HE
            ELdelete(rbnd); // mark the right HE for deletion - can't delete yet
                            // because there might be pointers to it in Hash Map
            pm = le; // set the pm variable to zero

            if (bot->coord.y > top->coord.y) // if the site to the left of the
                                             // event is higher than the site
            { // to the right of it, then swap them and set the 'pm' variable to
              // 1
                temp = bot;
                bot = top;
                top = temp;
                pm = re;
            }
            e = bisect(bot, top); // create an edge (or line) that is between
                                  // the two sites. This creates
            // the formula of the line, and assigns a line number to it
            bisector =
                HEcreate(e, pm); // create a HE from the edge 'e', and make it
                                 // point to that edge with its ELedge field
            ELinsert(llbnd, bisector); // insert the new bisector to the right
                                       // of the left HE
            endpoint(e, re - pm, v); // set one endpoint to the new edge to be
                                     // the vector point 'v'.
            // If the site to the left of this bisector is higher than the right
            // site, then this endpoint is put in position 0; otherwise in pos 1
            deref(v); // delete the vector 'v'

            // if left HE and the new bisector don't intersect, then delete the
            // left HE, and reinsert it
            if ((p = intersect(llbnd, bisector)) != (site*)NULL)
            {
                PQdelete(llbnd);
                PQinsert(llbnd, p, dist(p, bot));
            };

            // if right HE and the new bisector don't intersect, then reinsert
            // it
            if ((p = intersect(bisector, rrbnd)) != (site*)NULL)
            {
                PQinsert(bisector, p, dist(p, bot));
            };
        }
        else
            break;
    };

    for (lbnd = ELright(ELleftend); lbnd != ELrightend; lbnd = ELright(lbnd))
    {
        e = lbnd->ELedge;

        clip_line(e);
    };

    // cleanup();

    return true;
}

int scomp(const void* p1, const void* p2)
{
    terra::vec2 *s1 = (terra::vec2*)p1, *s2 = (terra::vec2*)p2;
    if (s1->y < s2->y)
        return (-1);
    if (s1->y > s2->y)
        return (1);
    if (s1->x < s2->x)
        return (-1);
    if (s1->x > s2->x)
        return (1);
    return (0);
}

/* return a single in-storage site */
site* voronoi_diagram_generator::nextone()
{
    site* s;
    if (siteidx < nsites)
    {
        s = &sites[siteidx];
        siteidx += 1;
        return (s);
    }
    else
        return ((site*)NULL);
}
