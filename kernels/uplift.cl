#ifdef
typedef double tfloat;
#else
typedef float tfloat;
#endif

kernel void uplift_update(global const tfloat* uplifts, global tfloat* heights)
{
    int i = get_global_id(0);
    heights[i] += uplifts[i];
}
