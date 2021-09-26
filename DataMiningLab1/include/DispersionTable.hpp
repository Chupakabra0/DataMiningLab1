#pragma once
#include <numeric>
#include <vector>

template<class T, class... U> requires
	std::is_arithmetic_v<T>&& (!std::is_same_v<T, bool>) && (!std::is_same_v<T, char>)
	&& (std::is_arithmetic_v<U> && ... ) && (!std::is_same_v<U, bool> && ...) && (!std::is_same_v<U, char> && ...)
class DispersionTable {
public:
	explicit DispersionTable(T value) {
		this->AddToAllValues(value);
		this->UpdateAllSums();
	}

	explicit DispersionTable(T firstValue, U... restValues) {
		this->AddToAllValues(firstValue);
		(this->AddToAllValues(static_cast<T>(restValues)), ...);
		this->UpdateAllSums();
	}

	void Clear() {
		this->values_.clear();
		this->squaredValues_.clear();

		this->sumValues_        = {};
		this->sumSquaredValues_ = {};
	}

	void SetNewValues(T value) {
		this->Clear();

		this->AddToAllValues(value);
		this->UpdateAllSums();
	}

	void SetNewValues(T firstValue, U... restValues) {
		this->Clear();

		this->AddToAllValues(firstValue);
		(this->AddToAllValues(static_cast<T>(restValues)), ...);
		this->UpdateAllSums();
	}

	[[nodiscard]] std::pair<std::vector<T>, std::vector<T>> GetAllInfo() const {
		std::pair result{ this->values_, this->squaredValues_ };
		result.first.push_back(this->sumValues_);
		result.second.push_back(this->sumSquaredValues_);

		return result;
	}

private:
	std::vector<T> values_;
	std::vector<T> squaredValues_;

	T sumValues_;
	T sumSquaredValues_;

	void AddToAllValues(T value) {
		this->values_.push_back(value);
		this->squaredValues_.push_back(std::pow(value, T{ 2.0 }));
	}

	void UpdateAllSums() {
		this->sumValues_        = this->CalculateSum(this->values_);
		this->sumSquaredValues_ = this->CalculateSum(this->squaredValues_);
	}

	template<class T>
	auto CalculateSum(const std::vector<T>& v) -> T {
		return std::accumulate(v.cbegin(), v.cend(), T{}, std::plus());
	}
};
