# Лабораторная работа №1

## Постановка задачи:

 - построить дисперсионную таблицу.
 - определить точечные оценки числовых характеристик случайных величин:
	 - математическое ожидание;
	 - дисперсия (смещенная, несмещенная);
	 - среднеквадратичное отклонение (смещенное, несмещенное).
 - найти доверительный интервал для математического ожидания в случае известной/неизвестной дисперсии.

Входные данные:

| Вариант №7 | | | | | | | | | | |
|--|--|--|--|--|--|--|--|--|--|--|
| $X_i$ | 5.1 | 7.2 | 2.4 | 5.1 | 11.2 | 12.2 | 10.1 | 4.8 | 2.3 | 1.5 |
| $Y_i$ | 18.7 | 28 | 16.2 | 25.5 | 34.1 | 44.2 | 35 | 23 | 8.1 | 10.7 |

## Выполнение:

### Инструментарий

Для написания программы был использован язык C++. Сама программа является консольным приложением. Для ввода входных данных используется файл с расширением ``.json``.

Пример корректного файла:

```json
{
	"data": [
		5.1, 7.2, 2.4, 5.1, 11.2, 12.2, 10.1, 4.8, 2.3, 1.5
	],
	"sigma": 1.0,
	"alpha": [
		0.95, 0.05
	],
	"freedomLevel": 5
}
```

``"data"`` - выборка данных;
``"sigma"`` - значение $\sigma$, при котором будет находиться доверительный интервал при известной дисперсии;
``"alpha"``- значения $\alpha$, при которых будут находится доверительные интервалы при известной и неизвестной дисперсии соответственно;
``"freedomLevel"`` - число степени свободы, при котором будет находиться доверительный интервал при неизвестной дисперсии;

### Реализация таблицы Стьюдента и таблицы нормального распределения

Обе таблицы были реализованы при помощи контейнера ``std::map``, который предоставляет интерфейс словаря: получение доступа к элементу при помощи уникального идентификатора.

Файл ``NormalTable.hpp`` :

```Cpp
#pragma once
#include <array>
#include <map>

class NormalTable {
public:
	~NormalTable() = default;

	static NormalTable& GetInstance() {
		static NormalTable instance;
		return instance;
	}

	[[nodiscard]] std::pair<double, double> GetElement(const double value) const {
		auto result = std::pair{ 0.0, 0.0 };

		if (value < this->map_.begin()->first) {
			return result;
		}
		if (value > std::prev(this->map_.end())->first) {
			return result;
		}

		const auto lowerBound = this->map_.lower_bound(value);
		const auto prev       = std::prev(lowerBound);

		return std::abs(value - prev->first) < std::abs(lowerBound->first - value) ? prev->second : lowerBound->second;
	}

private:
	std::array<double, 210u> totalTable_ {
	    0.5000, 0.5040, 0.5080, 0.5120, 0.5160, 0.5199, 0.5239, 0.5279, 0.5319, 0.5359, // 0.0
	    0.5398, 0.5438, 0.5478, 0.5517, 0.5557, 0.5596, 0.5636, 0.5675, 0.5714, 0.5753, // 0.1
	    0.5793, 0.5832, 0.5871, 0.5910, 0.5948, 0.5987, 0.6026, 0.6064, 0.6103, 0.6141, // 0.2
	    0.6179, 0.6217, 0.6255, 0.6293, 0.6331, 0.6368, 0.6406, 0.6443, 0.6480, 0.6517, // 0.3
	    0.6554, 0.6591, 0.6628, 0.6664, 0.6700, 0.6736, 0.6772, 0.6808, 0.6844, 0.6879, // 0.4
	    0.6915, 0.6950, 0.6985, 0.7019, 0.7054, 0.7088, 0.7123, 0.7157, 0.7190, 0.7224, // 0.5
	    0.7257, 0.7291, 0.7324, 0.7357, 0.7389, 0.7422, 0.7454, 0.7486, 0.7517, 0.7549, // 0.6
	    0.7580, 0.7611, 0.7642, 0.7673, 0.7704, 0.7734, 0.7764, 0.7794, 0.7823, 0.7852, // 0.7
	    0.7881, 0.7910, 0.7939, 0.7967, 0.7995, 0.8023, 0.8051, 0.8078, 0.8106, 0.8133, // 0.8
	    0.8159, 0.8186, 0.8212, 0.8238, 0.8264, 0.8289, 0.8315, 0.8340, 0.8365, 0.8389, // 0.9
	    0.8413, 0.8438, 0.8461, 0.8485, 0.8508, 0.8531, 0.8554, 0.8577, 0.8599, 0.8621, // 1.0
	    0.8643, 0.8665, 0.8686, 0.8708, 0.8729, 0.8749, 0.8770, 0.8790, 0.8810, 0.8830, // 1.1
	    0.8849, 0.8869, 0.8888, 0.8907, 0.8925, 0.8944, 0.8962, 0.8980, 0.8997, 0.9015, // 1.2
	    0.9032, 0.9049, 0.9066, 0.9082, 0.9099, 0.9115, 0.9131, 0.9147, 0.9162, 0.9177, // 1.3
	    0.9192, 0.9207, 0.9222, 0.9236, 0.9251, 0.9265, 0.9279, 0.9292, 0.9306, 0.9319, // 1.4
	    0.9332, 0.9345, 0.9357, 0.9370, 0.9382, 0.9394, 0.9406, 0.9418, 0.9429, 0.9441, // 1.5
	    0.9452, 0.9463, 0.9474, 0.9484, 0.9495, 0.9505, 0.9515, 0.9525, 0.9535, 0.9545, // 1.6
	    0.9554, 0.9564, 0.9573, 0.9582, 0.9591, 0.9599, 0.9608, 0.9616, 0.9625, 0.9633, // 1.7
	    0.9641, 0.9649, 0.9656, 0.9664, 0.9671, 0.9678, 0.9686, 0.9693, 0.9699, 0.9706, // 1.8
	    0.9713, 0.9719, 0.9726, 0.9732, 0.9738, 0.9744, 0.9750, 0.9756, 0.9761, 0.9767, // 1.9
	    0.9772, 0.9778, 0.9783, 0.9788, 0.9793, 0.9798, 0.9803, 0.9808, 0.9812, 0.9817  // 2.0
	//  0.0000  0.0100  0.0200, 0.0300  0.0400, 0.0500, 0.0600, 0.0700, 0.0800, 0.0900
	};

	std::map<double, std::pair<double, double>> map_{};

	NormalTable() {
		this->FillMap();
	}

	void FillMap() {
		auto totalIter = this->totalTable_.begin();

		for (auto i = 0.0; 2.1 - i > std::numeric_limits<double>::epsilon(); i += 0.1) {
			for (auto j = 0.0; 0.1 - j > std::numeric_limits<double>::epsilon(); j += 0.01) {
				this->map_[*totalIter++] = { i, j };
			}
		}
	}
};
```

Файл ``StudentsTable.hpp`` :

```Cpp
#pragma once
#include <array>
#include <numeric>
#include <map>

class StudentsTable {
private:
	static constexpr auto ALPHA_SIZE = 8u;
	static constexpr auto K_SIZE     = 35u;
	static constexpr auto TOTAL_SIZE = StudentsTable::ALPHA_SIZE * StudentsTable::K_SIZE;
public:
	~StudentsTable() = default;

	static StudentsTable& GetInstance() {
		static StudentsTable instance;
		return instance;
	}

	[[nodiscard]] double GetElement(const double sigma, const unsigned k) const {
		auto result = 0.0;

		if (k < this->kTable_.front() || k > this->kTable_.back()) {
			return result;
		}
		if (sigma > this->alphaTable_.front() || sigma < this->alphaTable_.back()) {
			return result;
		}

		const auto lowerBound = this->map_.lower_bound(k)->second.lower_bound(sigma);
		const auto prev = std::prev(lowerBound);

		return std::abs(sigma - prev->first) < std::abs(lowerBound->first - sigma) ? prev->second : lowerBound->second;
	}

private:
	std::array<double, StudentsTable::TOTAL_SIZE> totalTable_ {
		3.078, 6.314, 12.71, 31.82, 63.66, 127.3, 318.3, 636.6,   // k = 1
	    1.886, 2.920, 4.303, 6.965, 9.925, 14.09, 22.33, 31.60,   // k = 2
	    1.638, 2.353, 3.182, 4.541, 5.841, 7.453, 10.21, 12.92,   // k = 3
	    1.533, 2.132, 2.776, 3.747, 4.604, 5.598, 7.173, 8.610,   // k = 4
	    1.476, 2.015, 2.571, 3.365, 4.032, 4.773, 5.894, 6.869,   // k = 5
	    1.440, 1.943, 2.447, 3.143, 3.707, 4.317, 5.208, 5.959,   // k = 6
	    1.415, 1.895, 2.365, 2.998, 3.499, 4.029, 4.785, 5.408,   // k = 7
	    1.397, 1.860, 2.306, 2.698, 3.355, 3.893, 4.501, 5.041,   // k = 8
	    1.383, 1.833, 2.262, 2.821, 3.250, 3.690, 4.297, 4.781,   // k = 9
	    1.372, 1.812, 2.228, 2.764, 3.169, 3.581, 4.144, 4.587,   // k = 10
	    1.363, 1.796, 2.201, 2.718, 3.106, 3.497, 4.025, 4.437,   // k = 11
	    1.356, 1.782, 2.179, 2.681, 3.055, 3.428, 3.930, 4.318,   // k = 12
	    1.350, 1.771, 2.160, 2.650, 3.012, 3.372, 3.852, 4.221,   // k = 13
	    1.345, 1.761, 2.145, 2.624, 2.977, 3.326, 3.787, 4.140,   // k = 14
	    1.341, 1.753, 2.131, 2.602, 2.947, 3.286, 3.733, 4.073,   // k = 15
	    1.337, 1.746, 2.120, 2.583, 2.921, 3.252, 3.686, 4.015,   // k = 16
	    1.333, 1.740, 2.110, 2.567, 2.898, 3.222, 3.646, 3.965,   // k = 17
	    1.330, 1.734, 2.101, 2.552, 2.878, 3.197, 3.610, 3.922,   // k = 18
	    1.328, 1.729, 2.093, 2.539, 2.861, 3.174, 3.579, 3.833,   // k = 19
	    1.325, 1.725, 2.086, 2.528, 2.845, 3.153, 3.552, 3.850,   // k = 20
	    1.323, 1.721, 2.080, 2.518, 2.831, 3.135, 3.527, 3.819,   // k = 21
	    1.321, 1.717, 2.074, 2.508, 2.819, 3.119, 3.505, 3.792,   // k = 22
	    1.319, 1.714, 2.069, 2.500, 2.807, 3.104, 3.485, 3.768,   // k = 23
	    1.318, 1.711, 2.064, 2.492, 2.797, 3.091, 3.467, 3.745,   // k = 24
	    1.316, 1.708, 2.060, 2.485, 2.787, 3.078, 3.450, 3.725,   // k = 25
	    1.315, 1.706, 2.056, 2.479, 2.779, 3.067, 3.435, 3.707,   // k = 26
	    1.314, 1.703, 2.052, 2.473, 2.771, 3.057, 3.421, 3.689,   // k = 27
	    1.313, 1.701, 2.048, 2.467, 2.763, 3.047, 3.408, 3.674,   // k = 28
	    1.311, 1.699, 2.045, 2.462, 2.756, 3.038, 3.396, 3.660,   // k = 29
	    1.310, 1.697, 2.042, 2.457, 2.750, 8.030, 3.385, 3.646,   // k = 30
	    1.303, 1.684, 2.021, 2.423, 2.704, 2.971, 3.307, 3.551,   // k = 40
	    1.296, 1.671, 2.000, 2.390, 2.660, 2.915, 3.232, 3.460,   // k = 60
	    1.290, 1.660, 1.984, 2.864, 2.626, 2.871, 3.174, 3.390,   // k = 100
	    1.289, 1.658, 1.980, 2.358, 2.617, 2.860, 3.160, 3.373,   // k = 120
		1.282, 1.645, 1.960, 2.327, 2.576, 2.808, 3.091, 3.291 	  // k = 10'000
	//  0.200  0.100  0.050  0.025  0.012  0.005  0.002  0.001
	};

	std::array<unsigned, StudentsTable::K_SIZE> kTable_{};

	std::array<double, StudentsTable::ALPHA_SIZE> alphaTable_{};

	std::map<unsigned, std::map<double, double>> map_{};

	StudentsTable() {
		this->FillKTable();
		this->FillAlphaTable();
		this->FillMap();
	}

	void FillKTable() {
		constexpr auto offset = 30u;
		auto endOffsetIter    = std::next(this->kTable_.begin(), offset);

		std::iota(this->kTable_.begin(), endOffsetIter, 1u);

		*endOffsetIter++ = 40u;
		*endOffsetIter++ = 60u;
		*endOffsetIter++ = 100u;
		*endOffsetIter++ = 120u;
		*endOffsetIter++ = 10'000u;
	}

	void FillAlphaTable() {
		std::ranges::generate(this->alphaTable_, [alpha = 0.2]() mutable {
			const auto oldAlpha = alpha;
			alpha /= 2;
			return oldAlpha;
		});
	}

	void FillMap() {
		auto totalIter = this->totalTable_.begin();

		std::ranges::for_each(std::as_const(this->kTable_), [&totalIter, this](auto k) {
			std::ranges::for_each(std::as_const(this->alphaTable_),[&totalIter, k, this](auto alpha) {
					this->map_[k][alpha] = *totalIter++;
				}
			);
		});
	}
};
```

###  Выполнение основной части задания

Все функции расчёта были инкапсулированы в классе ``MathSolver``.  Экземпляры класса инициализируются диапазоном входных данных, которые будут подвержены анализу.

Файл ``MathSolve.hpp`` :

```Cpp
#pragma once
#include <complex>
#include <numeric>
#include <vector>

#include "NormalTable.hpp"
#include "StudentsTable.hpp"

class MathSolver {
public:
	template<class Iter>
	explicit MathSolver(Iter begin, Iter end) {
		this->AddRange(begin, end);
		this->UpdateAllSums();
	}

	void Clear() {
		this->values_.clear();
		this->squaredValues_.clear();

		this->sumValue_ = {};
		this->sumSquaredValue_ = {};
	}

	void SetNewValues(double value) {
		this->Clear();

		this->AddToAllValues(value);
		this->UpdateAllSums();
	}

	template<class... T> requires (std::is_same_v<T, double> && ...)
	void SetNewValues(double firstValue, T... restValues) {
		this->Clear();

		this->AddToAllValues(firstValue);
		(this->AddToAllValues(static_cast<double>(restValues)), ...);
		this->UpdateAllSums();
	}

	[[nodiscard]] std::pair<std::vector<double>, std::vector<double>> GetAllInfo() const {
		std::pair result{ this->values_, this->squaredValues_ };
		return result;
	}

	[[nodiscard]] std::pair<double, double> GetAllSum() const {
		std::pair result{ this->sumValue_, this->sumSquaredValue_ };
		return result;
	}

	[[nodiscard]] auto ExpectedValue() const -> double {
		return this->sumValue_ / this->values_.size();
	}

	[[nodiscard]] auto BiasedVarianceEstimate() const -> double {
		return this->sumSquaredValue_ / this->values_.size() -
			std::pow(this->sumValue_, 2.0) / std::pow(this->values_.size(), 2.0);
	}

	[[nodiscard]] auto UnbiasedVarianceEstimate() const -> double {
		return this->sumSquaredValue_ / (this->values_.size() - 1u) -
			std::pow(this->sumValue_, 2.0) / (this->values_.size() - 1u) / this->values_.size();
	}

	[[nodiscard]] auto BiasedStandardDeviationEstimate() const -> double {
		return std::sqrt(this->BiasedVarianceEstimate());
	}

	[[nodiscard]] auto UnbiasedStandardDeviationEstimate() const -> double {
		return std::sqrt(this->UnbiasedVarianceEstimate());
	}

	[[nodiscard]] auto TrustIntervalKnown(const double alpha, const double sigma) const
		-> std::pair<double, double> {
		const auto& normal = NormalTable::GetInstance();

		const auto [first, second] = normal.GetElement((alpha + 1.0) / 2.0);
		const auto epsilon = first + second;


		const auto expected = this->ExpectedValue();
		const auto offset   = epsilon * sigma / std::sqrt(this->values_.size());

		return { expected - offset, expected + offset };
	}

	[[nodiscard]] auto TrustIntervalUnknown(const double q, const unsigned freedomLevel) const
		-> std::pair<double, double> {
		const auto& student = StudentsTable::GetInstance();
		const auto epsilon = student.GetElement(q, freedomLevel);

		const auto expected  = this->ExpectedValue();
		const auto deviation = this->BiasedStandardDeviationEstimate();
		const auto offset    = epsilon * deviation / std::sqrt(this->values_.size() - 1u);

		return { expected - offset, expected + offset };
	}

private:
	double sumValue_{};
	double sumSquaredValue_{};

	std::vector<double> values_{};
	std::vector<double> squaredValues_{};

	void AddToAllValues(const double value) {
		this->values_.push_back(value);
		this->squaredValues_.push_back(std::pow(value, 2.0));
	}

	void UpdateAllSums() {
		this->sumValue_        = this->CalculateSum(this->values_);
		this->sumSquaredValue_ = this->CalculateSum(this->squaredValues_);
	}

	template<class T>
	auto CalculateSum(const std::vector<T>& v) -> T {
		return std::reduce(v.cbegin(), v.cend(), T{}, std::plus());
	}

	template<class Iter>
	void AddRange(Iter begin, Iter end) {
		for (; begin != end; ++begin) {
			this->AddToAllValues(*begin);
		}
	}
};
```

### Пример работы программы

Если запустить приложение без параметров, то мы увидим сообщение, которое предлагает нам добавить в аргументы командной строки файл с входными данными с расширением ``.json``.

![Запуск без файла](https://i.ibb.co/LYQnBTX/test-Without-File.png)

Запустим приложение с параметром командной строки ``x.json``

![Запуск с одним файлом](https://i.ibb.co/GTb723b/test-With-Onet-File.png)

Также в аргументы командной строки можно закидывать два и больше файлов. В случае, если файл не будет найден или он будет некорректно прочитан, будет выведено соответствующая ошибка.

Например, в примере ниже в программу были отправлены два корректных файла и один несуществующий.

![Запуск с тремя файлами. Часть 1](https://i.ibb.co/GJYC3pQ/test-With-Three-Files.png)
![Запуск с тремя файлами. Часть 2](https://i.ibb.co/nw1FSps/test-With-Three-Files.png)
