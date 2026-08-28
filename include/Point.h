#ifndef POINT_H
#define POINT_H


class Point
{
    public:
        Point();
        Point(int x, int y);
        int getX();
        int getY();
        int getZ();
        void setX(int x);
        void setY(int y);
        void setZ(int z);
        virtual ~Point();

    protected:

    private:
        int x, y, z;
};

#endif // POINT_H
