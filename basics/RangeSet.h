//------------------------------------------------------------------------------
/*
    This file is part of rippled: https://github.com/ripple/rippled
    Copyright (c) 2012, 2013 Ripple Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef RIPPLE_BASICS_RANGESET_H_INCLUDED
#define RIPPLE_BASICS_RANGESET_H_INCLUDED

#include <cstdint>
#include <map>
#include <string>

namespace ripple {

/** A sparse set of integers. */
// VFALCO TODO Replace with juce::SparseSet
class RangeSet
{
public:
    static const std::uint32_t absent = static_cast <std::uint32_t> (-1);

public:
    RangeSet () { }

    bool hasValue (std::uint32_t) const;
    bool hasRange(std::pair<std::uint32_t, std::uint32_t>) const;

    std::uint32_t getFirst () const;
    std::uint32_t getNext (std::uint32_t) const;
    std::uint32_t getLast () const;
    std::uint32_t getPrev (std::uint32_t) const;

    // largest number not in the set that is less than the given number
    std::uint32_t prevMissing (std::uint32_t) const;

    // Add an item to the set
    void setValue (std::uint32_t);

    // Add the closed interval to the set
    void setRange (std::uint32_t, std::uint32_t);

    void clearValue (std::uint32_t);

    std::string toString () const;

    /** Returns the sum of the Lebesgue measures of all sub-ranges. */
    std::size_t
    lebesgue_sum() const;

    /** Check invariants of the data.

        This is for diagnostics, and does nothing in release builds.
    */
    void checkInternalConsistency () const noexcept;

private:
    void simplify ();

private:
    using Map = std::map <std::uint32_t, std::uint32_t>;

    using const_iterator         = Map::const_iterator;
    using const_reverse_iterator = Map::const_reverse_iterator;
    using value_type             = Map::value_type;
    using iterator               = Map::iterator;

    static bool contains (value_type const& it, std::uint32_t v)
    {
        return (it.first <= v) && (it.second >= v);
    }

    static bool contains(value_type const& it, std::pair<std::uint32_t, std::uint32_t> vRange)
    {
        return (it.first <= vRange.first) && (it.second >= vRange.second);
    }
    // First is lowest value in range, last is highest value in range
    Map mRanges;
};

} // ripple

// #include <string>
// #include <boost/optional.hpp>
// #include <boost/icl/closed_interval.hpp>
// #include <boost/icl/interval_set.hpp>

// namespace ripple
// {

// /** A closed interval over the domain T.

//     For an instance ClosedInterval c, this represents the closed interval
//     (c.first(), c.last()).  A single element interval has c.first() == c.last().

//     This is simply a type-alias for boost interval container library interval
//     set, so users should consult that documentation for available supporting
//     member and free functions.
// */
// template <class T>
// using ClosedInterval = boost::icl::closed_interval<T>;

// /** Create a closed range interval

//     Helper function to create a closed range interval without having to qualify
//     the template argument.
// */
// template <class T>
// ClosedInterval<T>
// range(T low, T high)
// {
//     return ClosedInterval<T>(low, high);
// }

// /** A set of closed intervals over the domain T.

//     Represents a set of values of the domain T using the minimum number
//     of disjoint ClosedInterval<T>.  This is useful to represent ranges of
//     T where a few instances are missing, e.g. the set 1-5,8-9,11-14.

//     This is simply a type-alias for boost interval container library interval
//     set, so users should consult that documentation for available supporting
//     member and free functions.
// */
// template <class T>
// using RangeSet = boost::icl::interval_set<T, std::less, ClosedInterval<T>>;


// /** Convert a ClosedInterval to a styled string

//     The styled string is
//         "c.first()-c.last()"  if c.first() != c.last()
//         "c.first()" if c.first() == c.last()

//     @param ci The closed interval to convert
//     @return The style string
// */
// template <class T>
// std::string to_string(ClosedInterval<T> const & ci)
// {
//     if (ci.first() == ci.last())
//         return std::to_string(ci.first());
//     return std::to_string(ci.first()) + "-" + std::to_string(ci.last());
// }

// /** Convert the given RangeSet to a styled string.

//     The styled string represention is the set of disjoint intervals joined by
//     commas.  The string "empty" is returned if the set is empty.

//     @param rs The rangeset to convert
//     @return The styled string
// */
// template <class T>
// std::string to_string(RangeSet<T> const & rs)
// {
//     using ripple::to_string;

//     if (rs.empty())
//         return "empty";
//     std::string res = "";
//     for (auto const & interval : rs)
//     {
//         if (!res.empty())
//             res += ",";
//         res += to_string(interval);
//     }
//     return res;
// }

// /** Find the largest value not in the set that is less than a given value.

//     @param rs The set of interest
//     @param t The value that must be larger than the result
//     @param minVal (Default is 0) The smallest allowed value
//     @return The largest v such that minV <= v < t and !contains(rs, v) or
//             boost::none if no such v exists.
// */
// template <class T>
// boost::optional<T>
// prevMissing(RangeSet<T> const & rs, T t, T minVal = 0)
// {
//     if (rs.empty() || t == minVal)
//         return boost::none;
//     RangeSet<T> tgt{ ClosedInterval<T>{minVal, t - 1} };
//     tgt -= rs;
//     if (tgt.empty())
//         return boost::none;
//     return boost::icl::last(tgt);
// }
//  }  // namespace ripple
#endif