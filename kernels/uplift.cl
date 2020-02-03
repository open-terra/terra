kernel void uplift_update(global const double* uplifts, global double* heights)
{
    int i = get_global_id(0);
    heights[i] += uplifts[i];
}
