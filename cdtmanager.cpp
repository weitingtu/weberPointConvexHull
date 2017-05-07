#include "cdtmanager.h"
#include <QMap>
#include <QSet>

CDTManager::CDTManager(QObject *parent) : QObject(parent),
    _points(),
    _points_group_idx(),
    _group_idx(),
    _segments(),
    _lines(),
    _triangles()
{
}

void CDTManager::clear()
{
    _points.clear();
    _points_group_idx.clear();
    _group_idx.clear();
    _segments.clear();
    _lines.clear();
    _triangles.clear();
}

void CDTManager::cdt()
{
    if(_points.empty())
    {
        return;
    }

    _group_to_segments();

    struct triangulateio in  = _create_input();
    struct triangulateio mid = _create_mid();

    char options[] = "pczAen";
    triangulate(options, &in, &mid, (struct triangulateio *) NULL );

    _set_lines_by_edges(mid);
    _set_triangles(mid);

    free(in.pointlist);
    free(in.segmentlist);

    free(mid.pointlist);
    free(mid.trianglelist);
    free(mid.neighborlist);
    free(mid.segmentlist);
    free(mid.edgelist);
}

void CDTManager::_group_to_segments()
{
    for(int i = 0; i < _group_idx.size(); ++i)
    {
        for(int j = 0; j < _group_idx[i].size() - 1; ++j)
        {
            _segments.push_back(qMakePair(_group_idx[i][j], _group_idx[i][j + 1]));
        }
        if(_group_idx[i].size() > 1)
        {
            _segments.push_back(qMakePair(_group_idx[i].back(), _group_idx[i].front()));
        }
    }
}

struct triangulateio CDTManager::_create_input() const
{
    struct triangulateio in;
    in.numberofpoints = _points.size();
    in.numberofpointattributes = 0;
    in.pointlist = (REAL*) malloc(in.numberofpoints * 2 * sizeof(REAL));
    for(int i = 0; i < _points.size(); ++i)
    {
        in.pointlist[i * 2]     = _points[i].x();
        in.pointlist[i * 2 + 1] = _points[i].y();
    }
    in.numberofsegments = _segments.size();
    in.segmentlist = (int*) malloc((in.numberofsegments * 2 * sizeof(int)));
    for(int i = 0; i < _segments.size(); ++i)
    {
        in.segmentlist[i * 2]     = _segments[i].first;
        in.segmentlist[i * 2 + 1] = _segments[i].second;
    }
    in.numberofholes = 0;
    in.holelist = nullptr;
    in.numberofregions = 0;

    return in;
}

struct triangulateio CDTManager::_create_mid() const
{
    struct triangulateio mid;
    mid.pointlist = (REAL *) NULL;             /* Not needed if -N switch used. */
    mid.pointattributelist = (REAL *) NULL;    /* Not needed if -N switch used or number of point attributes is zero: */
    mid.pointmarkerlist = (int *) NULL;        /* Not needed if -N or -B switch used. */
    mid.trianglelist = (int *) NULL;           /* Not needed if -E switch used. */
    mid.triangleattributelist = (REAL *) NULL; /* Not needed if -E switch used or number of triangle attributes is zero: */
    mid.neighborlist = (int *) NULL;           /* Needed only if -n switch used. */
    mid.segmentlist = (int *) NULL;            /* Needed only if segments are output (-p or -c) and -P not used: */
    mid.segmentmarkerlist = (int *) NULL;      /* Needed only if segments are output (-p or -c) and -P and -B not used: */
    mid.edgelist = (int *) NULL;               /* Needed only if -e switch used. */
    mid.edgemarkerlist = (int *) NULL;         /* Needed if -e used and -B not used. */

    return mid;
}

void CDTManager::_set_lines_by_edges(const triangulateio& io)
{
    _lines.clear();
    for (int i = 0; i < io.numberofedges; i++)
    {
        int idx1 = io.edgelist[i * 2];
        int idx2 = io.edgelist[i * 2 + 1];
        if((_points_group_idx[idx1] == -1)||(_points_group_idx[idx2] == -1)||(_points_group_idx[idx1] != _points_group_idx[idx2]))
        {
            _lines.push_back(QLineF(_points[idx1], _points[idx2]));
        }
    }
}

void CDTManager::_set_triangles(const triangulateio& io)
{
    _triangles.clear();
    for (int i = 0; i < io.numberoftriangles; i++)
    {
        Triangle t;
        for (int j = 0; j < io.numberofcorners; j++)
        {
            int idx = io.trianglelist[i * io.numberofcorners + j];
            t.points[j] = _points[idx];
            t.indices[j]  = idx;
        }

        for (int j = 0; j < 3; j++)
        {
            t.neighbors[j] = io.neighborlist[i * 3 + j];
        }
        t.idx = std::numeric_limits<int>::max();
        for(int i = 0; i < 3; ++i)
        {
            if(_points_group_idx[t.indices[i]] < 0)
            {
                continue;
            }
            t.idx = std::min(t.idx, _points_group_idx[t.indices[i]]);
        }
        t.set_center();
        _triangles.push_back(t);
    }
}
