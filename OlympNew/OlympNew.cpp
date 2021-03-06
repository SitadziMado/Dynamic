#include <algorithm> 
#include <array> 
#include <bitset> 
#include <fstream> 
#include <functional> 
#include <iostream> 
#include <limits>
#include <list> 
#include <map> 
#include <memory> 
#include <numeric> 
#include <random> 
#include <set> 
#include <sstream> 
#include <string> 
#include <type_traits> 
#include <unordered_map> 
#include <unordered_set> 
#include <valarray> 
#include <vector> 
#include <queue> 

#include <cmath> 
#include <cctype> 
#include <clocale> 
#include <cstring> 

using i64 = long long;
using u64 = unsigned long long;
using std::cin;
using std::cout;

#if true
template<typename T>
bool odd(T n)
{
    static_assert(std::is_arithmetic<T>::value, "Требуется арифметический тип");

    return n & 1;
}

template<typename T>
bool even(T n)
{
    return !odd(n);
}

template<typename T, typename U>
bool divides(T a, U b)
{
    static_assert(std::is_arithmetic<T>::value, "Требуется арифметический тип");
    static_assert(std::is_arithmetic<U>::value, "Требуется арифметический тип");

    return !(a % b);
}

template<typename T>
std::valarray<T> readValarray(size_t size)
{
    auto result = std::valarray<T>(size);

    for (size_t i = 0; i < size; ++i)
    {
        T t;
        std::cin >> t;
        result[i] = t;
    }

    return result;
}
#endif

void solve(std::istream& in, std::ostream& out);

int main()
{
    std::ios_base::sync_with_stdio(false);

    std::setlocale(LC_ALL, "Russian");

    auto in = std::ifstream("input.txt");

    solve(in, cout);

#ifdef HOME
    std::system("pause");
#endif // HOME

    return 0;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, std::vector<T>& data)
{
	auto it = data.cbegin();

	stream << *it++;

	for (; it != data.cend(); ++it)
	{
		stream << " " << *it;
	}

	return stream;
}

struct Record
{
    Record() {}
    Record(double profitability, std::vector<size_t> organization) :
        profitability_(profitability),
        invested_(organization)
    {}

    double profitability_;
    std::vector<size_t> invested_;
};

bool operator<(const Record& lhs, const Record& rhs)
{
	return lhs.profitability_ < rhs.profitability_;
}

std::vector<Record> nextSlice(std::vector<Record>& fst, std::vector<Record>& snd, i64 mills)
{
    std::vector<Record> result;
    result.reserve(mills);

    for (i64 i = 0; i <= mills; ++i)
    {
        double max = -std::numeric_limits<double>::max();
        Record* rec = nullptr;
        i64 cnt = 0;

        for (i64 j = 0; j <= i; ++j)
        {
            auto cur = fst[j].profitability_ + snd[i - j].profitability_;

            if (max < cur)
            {
                max = cur;
                rec = &fst[j];
                cnt = snd[i - j].invested_.back();
            }
        }

        result.push_back(Record(max, rec->invested_));
        result.back().invested_.push_back(cnt);
    }

    return result;
}

void solve(std::istream& in, std::ostream& out)
{
    i64 mills;
    size_t n;

    in >> n >> mills;

    auto orgs = std::vector<std::vector<Record>>(
        n,
        std::vector<Record>(mills + 1)
    );
    
    for (i64 i = 0; i <= mills; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
			auto& cur = orgs[j][i];

            double t;
            in >> t;
            cur.profitability_ = t;
            cur.invested_.push_back(i);
        }
    }

    auto result = std::vector<std::vector<Record>>();
    result.push_back(nextSlice(orgs[0], orgs[1], mills));

    for (size_t i = 1; i < n - 1; ++i)
    {
        result.push_back(
            nextSlice(result[i - 1], orgs[i + 1], mills)
        );
    }

    auto row = result.back();
    Record* rec = &row.front();

    for (auto&& a : row)
    {
        if (*rec < a)
        {
            rec = &a;
        }
    }

    cout << "Профит: " << rec->profitability_ << " млн" << std::endl;
    cout << "Инвестиции по конторам: (";

	cout << rec->invested_;

    cout << ")" << std::endl;
}