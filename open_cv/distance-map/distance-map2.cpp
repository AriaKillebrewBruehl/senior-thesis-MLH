#include "distance-map2.hpp"

distMap distanceMap(std::string pathEdges, cv::Mat imgEdges,
                    std::string pathIsos, cv::Mat imgIsos, bool saving) {
    // read images and resize
    cv::Mat edges;
    edges = read(pathEdges, imgEdges);
    assert(!edges.empty());

    if (edges.type() == 0) {
        edges.convertTo(edges, 0);
    }

    cv::Mat isos;
    isos = read(pathIsos, imgIsos);
    assert(!isos.empty());

    if (isos.type() == 0) {
        isos.convertTo(isos, 0);
    }

    cv::Mat edgedists = sample_seeds(edges, pathEdges, true, false);
    cv::Mat isosdists = sample_seeds(isos, pathIsos, true, false);

    cv::Mat distances = cv::Mat::zeros(edges.rows, edges.cols, CV_32SC1);

    int edge_weight = 1;
    int isos_weight = 2;

    for (int i = 0; i < distances.rows; i++) {
        for (int j = 0; j < distances.cols; j++) {
            float edge_dist = edgedists.at<int32_t>(i, j) / edge_weight;
            float isos_dist = isosdists.at<int32_t>(i, j) / isos_weight;
            if (isos_dist < edge_dist) {
                distances.at<int32_t>(i, j) = int32_t(isos_dist);
            } else {
                distances.at<int32_t>(i, j) = int32_t(edge_dist);
            }
        }
    }
    // save image
    if (saving) {
        save(distances, pathEdges, "-dists-heavy-isos");
    }

    distMap dists;
    dists.distances = distances;

    return dists;
}