# Poisson Disk Sample
A Poisson-Disk sample is a random set of evenly spaced points in a arbitrary dimension, this creates a more natural blue noise pattern.

My implementation is based on that of Jason Davies which intern uses research from Robert Bridson to have a _O(n)_ algorithm.

## Implementation
### Initialise data structures
So to achive the _O(n)_ complexity we are using a spatially accelerated data structure, so first we need to initialise this.

In order to create our structure we need to compute our sell size which is "_cell size = radius * sqrt(1/2)_" so now we can initialise the array of a size "_ceiling size x * size y * cell size_", with the hashtable sizes being that of _x_ and _y_ over _cell size_.
```CPP
std::unique_ptr<int64_t[]> hashtable(new int64_t[hashtableX * hashtableY]);
```

We also have a list of created points and a list of the active indexs in the list of points.
```CPP
std::vector<cp::Vector3> points;
std::vector<int64_t> active;
```

### The first point on the grid
For this we need to first create a random device and a random engine in this case the Standard Mersene Twister engine which shall be seeded by our random device. These shall also be used later for generating points on a circle.
```CPP
std::random_device rd;
std::mt19937 gen(rd());
```

Now we have our random engine wew can create a random distribution within the bounds of the grid and thus sample to random points to get our first position.
```CPP
std::uniform_real_distribution<> dis(0.0, (double)std::min(size_x, size_y));
points.push_back(cp::Vector3(dis(gen), dis(gen)));
active.push_back(0);
```

We can also set the valid bounds for the next point as the "_radius_" to the "_size - radius_".
```CPP
double bx0 = radius;
double by0 = radius;
double bx1 = size_x - radius;
double by1 = size_y - radius;
```

### Creating the rest of the points
So now we shall loop until the list of active indexs is empty.

We are going to need a new random number generator here creating a real number between 0 and 1 thus being a normal value.
```CPP
std::uniform_real_distribution<> normal(0.0, 1.0);
```

Now we need to pick a random index from the list of indexs.
```CPP
int64_t i = normal(gen) * active.size();
```

From this indexwe shall now sample attempt to create a new point around the current point, we will give up if the count passes that of the sample count.

First we generate a random radian on the circle centered around the current point.
Then a random radius for the circle between the inner and outer bounds.
Now we can compute the position of the point on the circle.
```CPP
double inner = this->radius * this->radius;
double outer = (4 * inner) - inner;

double theta = normal(gen) * 2 * pi; // Random radian on the circumference of the circle
double r = utils::fast_sqrt((normal(gen) * outer) + inner); // Random radius of the circle between r^2 and 4r
cp::Vector3 point(  points[i].x + (r * std::cos(theta)),
                    points[i].y + (r * std::sin(theta)));
```

Now that we have a new point to test we can check if the point lies within the bounds of the sample.
```CPP
if (bx0 <= point.x && point.x <= bx1 && by0 <= point.y && point.y <= by1)
```

As we now know the point is valid or not we can proceed to check if it is near to any of it's neighbours if so then the point is discarded.
For this we divide the position by the cell size to get a base index of x and y, we then create the bounds of the surrounding "hashes".
```CPP
const int64_t n = 2; // Area to be explored for neighbours
int64_t x = point.x / cellSize;
int64_t y = point.y / cellSize;

// Calculate the bounds for the possible neighbours in the hashmap
int64_t x0 = std::max<int64_t>(x - n, 0);
int64_t y0 = std::max<int64_t>(y - n, 0);
int64_t x1 = std::min<int64_t>(x + n + 1, hashtableX);
int64_t y1 = std::min<int64_t>(y + n + 1, hashtableY);
```
Then the all of the location are visited via a loop and we check if the location holds a point and if so whether the new point is to close.
```CPP
int64_t index = hashtable[(y * size_y) + x];
if (index == -1)
{
    continue;
}

// If the point is to close to neighbour this point should be ignored
if (cp::Vector3::Distance2dSquared(point, this->points[index]) <= inner)
{
    break;
}
```

Finally once all these checks have passed we can add the current point to the points list and add its index to the list of active indexs, and update the hashtable.
```CPP
points.push_back(point);
count++;
active.push_back(count);
hashtable
[
    std::max((int64_t)(point.y / cellSize) * size_y, 0ll) +
    std::max((int64_t)(point.x / cellSize), 0ll)
] = count;
```

If no point was created then the current point is removed from the active list.
```CPP
if (j == this->samples)
{
    active.erase(active.begin() + i);
}
```

## References
- Robert Bridson, Fast Poisson Disk Sampling in Arbitrary Dimensions, 2006
- Jason Davies, Poisson-Disk Sampling