struct Point {
    int x;
    int y;
};

int dist(Point p, Point q) {
    int dx = p.x - q.x;
    int dy = p.y - q.y;
    return dx * dx + dy * dy;
}
