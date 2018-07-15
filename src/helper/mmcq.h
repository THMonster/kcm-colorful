#ifndef MMCQ_H
#define MMCQ_H

#include <QImage>
#include <QtCore>
#include <unordered_map>
#include <vector>

class VBox
{
public:
    VBox();
    explicit VBox(int r1, int r2, int g1, int g2, int b1, int b2);
    ~VBox();


    int r1, g1, b1, r2, g2, b2;
};


class MMCQ
{
public:
    explicit MMCQ(QString file);
    ~MMCQ();

    QList<QColor> get_palette(int color_count=10, int quality=10);
    int get_color_index(int r, int g, int b);

private:
    int sigbits = 5;
    int rshift = 8 - sigbits;
    double fract_by_populations = 0.75;
    int max_iter = 1000;
    QImage *img;
    int h = 0;
    int w = 0;
    std::vector<QRgb> pixels;
    std::unordered_map<int, int> histo;

    int get_vbox_color_sum(VBox v);
    void calc_histo();
    VBox gen_vbox();
    std::vector<VBox> do_median_cut(VBox v);
    QColor get_vbox_avg(VBox v);
    std::vector<VBox> quantize(int max_color);

    template<typename T>
    void quantize_iter(T &t, int target);

};




#endif
