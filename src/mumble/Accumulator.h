#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include <vector>
#include <limits>

/**
 * @brief The Accumulator class
 *        A header-only rough imlpementation of boost::accumulators to avoid boost dependancy.
 */

using acc_t = double;

class Accumulator
{
public:
    Accumulator(const std::vector<acc_t>& qPositions = {})
        : m_count(0),
          m_sum(0.0),
          m_sumOfSquares(0.0),
          m_quantilePositions(qPositions),
          m_markers()
    {
        if (!m_quantilePositions.empty())
            initExtendedP2();
    }

    void add(acc_t value)
    {
        m_count++;
        m_sum += value;

        if (m_count > 1)
        {
            acc_t delta = value - mean();
            m_sumOfSquares += delta * (value - mean());
        }

        if (!m_quantilePositions.empty())
            updateExtendedP2(value);
    }

    acc_t mean() const
    {
        return m_count > 0 ? m_sum / (acc_t)m_count : 0.0;
    }

    size_t count() const
    {
        return m_count;
    }

    acc_t variance() const
    {
        return (m_count > 1) ? (m_sumOfSquares / (acc_t)(m_count - 1)) : 0.0;
    }

    std::vector<acc_t> quantile() const
    {
        std::vector<acc_t> results;

        for (const auto& marker : m_markers)
            results.push_back(marker.q);

        return results;
    }

private:
    struct Marker
    {
        acc_t q;      // Estimated quantile value
        int n;        // Marker position
        acc_t np;     // Desired marker position
        acc_t dn;     // Increment
    };

private:
    void initExtendedP2()
    {
        for (acc_t q : m_quantilePositions)
        {
            m_markers.push_back({
                                    std::numeric_limits<acc_t>::quiet_NaN(),
                                    1,
                                    (q * (acc_t)m_count),
                                     0.0
                                });
        }
    }

    void updateExtendedP2(acc_t value)
    {
        if (m_markers.empty())
            return;

        if (std::isnan(m_markers.at(0).q))
        {
            for (auto& marker : m_markers)
            {
                marker.q = value;
                marker.n = 1;
                marker.np = marker.n;
                marker.dn = (acc_t)m_count * marker.np;
            }

            return;
        }

        for (auto& marker : m_markers)
        {
            if (value < marker.q)
                marker.n++;

            marker.np += marker.dn / (acc_t)m_count;
        }

        for (size_t i = 1; i < m_markers.size() - 1; ++i)
        {
            acc_t d = getMarkerDesiredPositions(m_markers.at(i));

            if ((d >= 1 && m_markers.at(i + 1).n - m_markers.at(i).n > 1) ||
                (d <= -1 && m_markers.at(i).n - m_markers.at(i - 1).n > 1))
            {
                m_markers[i].q += d * (m_markers.at(i + 1).q - m_markers.at(i - 1).q) / (m_markers.at(i + 1).n - m_markers.at(i - 1).n);
                m_markers[i].n += (size_t)d;
            }
        }
    }

    int getMarkerDesiredPositions(const Marker& marker) const
    {
        return static_cast<int>(std::round(marker.np - marker.n));
    }

private:
    size_t m_count;
    acc_t  m_sum;
    acc_t  m_sumOfSquares;
    std::vector<acc_t> m_quantilePositions;
    std::vector<Marker> m_markers;
};


#endif // ACCUMULATOR_H
