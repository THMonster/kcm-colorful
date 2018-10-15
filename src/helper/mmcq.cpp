#include <QImage>
#include <QRgb>
#include <queue>
#include <algorithm>
#include <QDebug>
#include "mmcq.h"


MMCQ::MMCQ(QString file)
{

    QImage i(file);
    if (i.isNull()) {
        qDebug() << "ERROR: Invalid image file!"; //Invalid image file.
        exit(1);
    }
    img = new QImage(i.convertToFormat(QImage::Format_RGBA8888));
    h = img->height();
    w = img->width();
}

MMCQ::~MMCQ()
{
    delete img;
}

QList<QColor> MMCQ::get_palette(int color_count, int quality)
{
    pixels.clear();
    histo.clear();
    QList<QColor> ret;
    for (int i = 0; i < w; i = i + quality)
        for (int j = 0; j < h; j++) {
            if (qAlpha(img->pixel(i, j)) >= 125) {
                if (qRed(img->pixel(i, j)) <= 250 && qGreen(img->pixel(i, j)) <= 250 && qBlue(img->pixel(i, j)) <= 250) {
                    pixels.push_back(img->pixel(i, j));
                }
            }
        }

    auto colors = quantize(color_count);
    for (const auto &c : colors) {
        ret.push_back(get_vbox_avg(c));
    }
    return ret;
}

int MMCQ::get_color_index(int r, int g, int b)
{
    return (r << (2 * sigbits)) + (g << sigbits) + b;
}

int MMCQ::get_vbox_color_sum(VBox v)
{
    int npix = 0;
    int index = 0;
    for (int i = v.r1; i <= v.r2; i++)
        for (int j = v.g1; j <= v.g2; j++)
            for (int k = v.b1; k <= v.b2; k++) {
                index = get_color_index(i, j, k);
                npix += histo[index];
            }
    return npix;
}

void MMCQ::calc_histo()
{
    int index = 0;
    for (unsigned int i = 0; i < pixels.size(); i++) {
        index = get_color_index(qRed(pixels[i]) >> rshift, qGreen(pixels[i]) >> rshift, qBlue(pixels[i]) >> rshift);
        histo[index]++;
    }

}

VBox MMCQ::gen_vbox()
{
    int rval, gval, bval;
    int rmin = 1000000;
    int rmax = 0;
    int gmin = 1000000;
    int gmax = 0;
    int bmin = 1000000;
    int bmax = 0;
    for (unsigned int i = 0; i < pixels.size(); i++) {
        rval = qRed(pixels[i]) >> rshift;
        gval = qGreen(pixels[i]) >> rshift;
        bval = qBlue(pixels[i]) >> rshift;

        rmin = std::min(rval, rmin);
        rmax = std::max(rval, rmax);
        gmin = std::min(gval, gmin);
        gmax = std::max(gval, gmax);
        bmin = std::min(bval, bmin);
        bmax = std::max(bval, bmax);
    }

    VBox vbox(rmin, rmax, gmin, gmax, bmin, bmax);
    return vbox;
}

std::vector<VBox> MMCQ::do_median_cut(VBox v)
{
    std::vector<VBox> ret;

    if (get_vbox_color_sum(v) == 1) {
        ret.push_back(v);
        return ret;
    }

    std::vector<int> side;
    side.push_back(v.r2 - v.r1);
    side.push_back(v.g2 - v.g1);
    side.push_back(v.b2 - v.b1);

    auto max_side = std::max_element(side.begin(), side.end());

    int sub_sum = 0;
    int total = 0;
    std::unordered_map<int, int> partial_sum;
    int index = 0;
    int dim1 = 0;
    int dim2 = 0;

    if (*max_side == side[0]) {
        dim1 = v.r1;
        dim2 = v.r2;
        for (int i = v.r1; i <= v.r2; i++) {
            for (int j = v.g1; j <= v.g2; j++)
                for (int k = v.b1; k <= v.b2; k++) {
                    index = get_color_index(i, j, k);
                    sub_sum += histo[index];
                }
            total += sub_sum;
            partial_sum[i] = total;
        }
    } else if (*max_side == side[1]) {
        dim1 = v.g1;
        dim2 = v.g2;
        for (int i = v.g1; i <= v.g2; i++) {
            for (int j = v.r1; j <= v.r2; j++)
                for (int k = v.b1; k <= v.b2; k++) {
                    index = get_color_index(i, j, k);
                    sub_sum += histo[index];
                }
            total += sub_sum;
            partial_sum[i] = total;
        }
    } else {
        dim1 = v.b1;
        dim2 = v.b2;
        for (int i = v.b1; i <= v.b2; i++) {
            for (int j = v.r1; j <= v.r2; j++)
                for (int k = v.g1; k <= v.g2; k++) {
                    index = get_color_index(i, j, k);
                    sub_sum += histo[index];
                }
            total += sub_sum;
            partial_sum[i] = total;
        }
    }


    for (int i = dim1; i <= dim2; i++) {
        if (partial_sum[i] > (total / 2)) {
            int left = i - dim1;
            int right = dim2 - i;
            int d2 = dim1;
            if (left <= right) {
                if ((dim2 - 1) > (i + (right / 2))) {
                    d2 = i + (right / 2);
                } else {
                    d2 = dim2 -1;
                }
            } else {
                if ((dim1) < (i - 1 - (left / 2))) {
                    d2 = i - 1 - (left / 2);
                } else {
                    d2 = dim1;
                }
            }

            int count2 = 0;
            while (!partial_sum[d2]) {
                d2++;
                count2 = total - partial_sum[d2];
            }
            while ((count2 == 0) && partial_sum[d2]) {
                d2--;
                count2 = total - partial_sum[d2];
            }

            VBox v1 = v;
            VBox v2 = v;
            if (*max_side == side[0]) {
                v1.r2 = d2;
                v2.r1 = d2 + 1;
            } else if (*max_side == side[1]) {
                v1.g2 = d2;
                v2.g1 = d2 + 1;
            } else {
                v1.b2 = d2;
                v2.b1 = d2 + 1;
            }
            ret.push_back(v1);
            ret.push_back(v2);
            return ret;
        }
    }
    return ret;
}

QColor MMCQ::get_vbox_avg(VBox v)
{
    int ntot = 0;
    int mult = 1 << (8 - sigbits);
    int r_sum = 0;
    int g_sum = 0;
    int b_sum = 0;
    int r_avg = 0;
    int g_avg = 0;
    int b_avg = 0;

    int index = 0;
    int hval = 0;
    for (int i = v.r1; i <= v.r2; i++)
        for (int j = v.g1; j <= v.g2; j++)
            for (int k = v.b1; k <= v.b2; k++) {
                index = get_color_index(i, j, k);
                hval = histo[index];
                ntot += hval;
                r_sum += hval * (i + 0.5) * mult;
                g_sum += hval * (j + 0.5) * mult;
                b_sum += hval * (k + 0.5) * mult;
            }
    if (ntot) {
        r_avg = int(r_sum / ntot);
        g_avg = int(g_sum / ntot);
        b_avg = int(b_sum / ntot);
    } else {
        r_avg = int(mult * (v.r1 + v.r2 + 1) / 2);
        g_avg = int(mult * (v.g1 + v.g2 + 1) / 2);
        b_avg = int(mult * (v.b1 + v.b2 + 1) / 2);
    }

    QColor c(r_avg, g_avg, b_avg);
    return c;
}

std::vector<VBox> MMCQ::quantize(int max_color)
{
    std::vector<VBox> ret;
    calc_histo();

    auto vbox = gen_vbox();

    auto cmp1 = [&](VBox left, VBox right) {
        auto left_val = get_vbox_color_sum(left);
        auto right_val = get_vbox_color_sum(right);
        return left_val < right_val;
    };
    std::priority_queue<int, std::vector<VBox>, decltype(cmp1)> pq1(cmp1);
    pq1.push(vbox);

    quantize_iter<decltype(pq1)>(pq1, max_color * fract_by_populations);

    auto cmp2 = [&](VBox left, VBox right) {
        auto left_val = get_vbox_color_sum(left);
        auto right_val = get_vbox_color_sum(right);
        left_val = left_val * (left.r2 - left.r1) * (left.g2 - left.g1) * (left.b2 - left.b1);
        right_val = right_val * (right.r2 - right.r1) * (right.g2 - right.g1) * (right.b2 - right.b1);
        return left_val < right_val;
    };
    std::priority_queue<int, std::vector<VBox>, decltype(cmp2)> pq2(cmp2);
    while (!pq1.empty()) {
        pq2.push(pq1.top());
        pq1.pop();
    }
    quantize_iter<decltype(pq2)>(pq2, max_color - pq2.size() + 1);

    while (!pq2.empty()) {
        ret.push_back(pq2.top());
        pq2.pop();
    }
    return ret;
}

template<typename T>
void MMCQ::quantize_iter(T &t, int target)
{
    int n_color = 1;
    int i = 0;
    while (i < max_iter) {

        auto v = t.top();
        t.pop();
        if (!get_vbox_color_sum(v)) {
            t.push(v);
            i++;
            continue;
        }
        auto vboxes = do_median_cut(v);
        if (vboxes.empty()) {
            return;
        }
        t.push(vboxes[0]);
        if (vboxes.size() > 1) {
            t.push(vboxes[1]);
            n_color++;
        }

        if (n_color >= target) {
            return;
        }

        if (i > max_iter) {
            qDebug() << "meet max";
            return;
        }
        i++;
    }
}


VBox::VBox()
{
    r1 = 0;
    g1 = 0;
    b1 = 0;
    r2 = 0;
    g2 = 0;
    b2 = 0;
}

VBox::VBox(int r1, int r2, int g1, int g2, int b1, int b2)
{
    this->r1 = r1;
    this->g1 = g1;
    this->b1 = b1;
    this->r2 = r2;
    this->g2 = g2;
    this->b2 = b2;
}

VBox::~VBox()
{

}


