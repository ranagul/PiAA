#include <iostream>
#include <vector>

struct point
{
    point(size_t x = 0, size_t y = 0) : x(x), y(y){}
    size_t x;
    size_t y;
};

struct answer
{
    answer(point p = point(), size_t length = 0) : coord(p), size(length) {}
    size_t size;
    point coord;
};

//Вывод ответа
void printAnswer( std::vector<answer> out )
{
    std::cout << out.size() << '\n';
    for( size_t i = 0; i < out.size(); i++ )
        std::cout << out[i].coord.x << ' ' << out[i].coord.y << ' ' << out[i].size << '\n';
    std::cout << '\n';
}

//Рисование квадрата
size_t draw( std::vector< std::vector<int> >* in, size_t limit, point p, size_t sq_num )
{
    limit = in->size() < limit + p.x ? in->size() - p.x : limit;//??
    limit = in->size() < limit + p.y ? in->size() - p.y : limit;//??

    size_t i = p.y;
    size_t j = p.x;

    for( i = p.y; i < p.y + limit; i++)
    {
        for(  j = p.x; j < p.x + limit; j++)
        {
            if( (*in)[i][j] )
            {
                limit = j - p.x;
            }
                    else
            {
                (*in)[i][j] = sq_num;
            }
        }
    }
    return limit;
}

//Must be optimized
//Поиск свободной точки
bool findfree(std::vector< std::vector<int> > in, point* p)
{
    for( size_t i = 0; i < in.size(); i++ )
        for( size_t j = 0; j < in.size(); j++ )
            if (!in[i][j])
            {
                *p = point(j, i);
                return true;
            }
    return false;
}

//Проверка на отсутсвие пробелов по периметру
bool checkFit( std::vector< std::vector<int> > in, point p, size_t size  )
{
    int res = 0;

    //Check bottom
    if( p.y + size == in.size() )
        res += size;
    else
        for( size_t mov = p.x; mov < p.x+size; mov++ )
            res += in[p.y + size][mov] ? 1 : 0;

    //Check left
    if( !p.x )
        res += size;
    else
        for( size_t mov = p.y; mov < p.y+size; mov++ )
            res += in[mov][p.x - 1] ? 1 : 0;

    //Check right
    if( p.x + size == in.size() )
        res += size;
    else
        for( size_t mov = p.y; mov < p.y+size; mov++ )
            res += in[mov][p.x + size] ? 1 : 0;
    res += size; //for skies

    if( res >= size*3 )
        return true;
    else
        return false;
    //return (((res/3) == (size*3)) ? true : false);
}

//Вывод матрицы
void print(std::vector<std::vector<int> > data)
{
    for( unsigned int i = 0; i<data.size(); i++ )
    {
        for( unsigned int j = 0; j<data.size(); j++ )
            std::cout << data[i][j] << ' ';
        std::cout << '\n';
    }
    std::cout << '\n';
}

//Основная рекурсивная функция
void trace( std::vector< std::vector<int> > in , size_t limit, size_t colour, std::vector<answer>* ans, std::vector<answer> out )
{
    size_t clent = 0;
    point p(0,0);

    bool fitness = true;
    bool freespace = true;

    //std::vector<point> out;
    std::vector< std::vector<int> > beforelast = in;

    //first one is different
    if( findfree(in, &p) )
    {
        clent = draw(&in, limit, p, colour);
        fitness = checkFit( in, p, clent );
    }

    if( !fitness && limit > 1 )
    {
        trace(beforelast, limit-1, colour, ans, out);
        //bufmas.push_back(bufans);
    }
    out.push_back(answer(p, clent));
    ++colour;
    findfree(in, &p);
    //print(in);


    while( freespace )
    {
        beforelast = in;
        clent = draw(&in, in.size() - 1, p, colour);


        fitness = checkFit( in, p, clent );
        if( !fitness && limit > 1 )
        {
            trace(beforelast, limit-1, colour, ans, out);
            //bufmas.push_back(bufans);
        }
        out.push_back(answer(p, clent));
        freespace = findfree( in, &p );
        ++colour;
    }
    //print(in);
    //std::cout << colour << '\n';

    //printAnswer(out);

    if( out.size() < ans->size() )
        *ans = out;

}

int main( void )
{
    int size;
    std::cin >> size;
    if( size % 2 == 0 )
    {
        std::cout << '4';
        std::cout << "0 0" << '\n';
        std::cout << 0 << ' ' << size/2 << '\n';
        std::cout << size/2 << ' ' << 0 << '\n';
        std::cout << size/2 << ' ' << size/2 << '\n';
        return 0;
    }
    else
    {
        std::vector<answer> ans;
        std::vector< std::vector<int> > data(size, std::vector<int>(size, 0));
        std::vector<answer> a;

        ans.resize(40);
        trace(data, size - 1, 1, &ans, a);

        printAnswer(ans);
    }
}
