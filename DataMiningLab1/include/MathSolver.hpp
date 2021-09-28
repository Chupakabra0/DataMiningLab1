#pragma once
#include <complex>
#include <numeric>
#include <vector>

#include "NormalTable.hpp"
#include "StudentsTable.hpp"

template<class T, class... U> requires
	std::is_arithmetic_v<T> && (!std::is_same_v<T, bool>) && (!std::is_same_v<T, char>)
	&& (std::is_arithmetic_v<U> && ...) && (!std::is_same_v<U, bool> && ...) && (!std::is_same_v<U, char> && ...)
class MathSolver {
public:
	explicit MathSolver(T value) {
		this->AddToAllValues(value);
		this->UpdateAllSums();
	}

	explicit MathSolver(T firstValue, U... restValues) {
		this->AddToAllValues(firstValue);
		(this->AddToAllValues(static_cast<T>(restValues)), ...);
		this->UpdateAllSums();
	}

	void Clear() {
		this->values_.clear();
		this->squaredValues_.clear();

		this->sumValue_ = {};
		this->sumSquaredValue_ = {};
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
		return result;
	}

	[[nodiscard]] std::pair<T, T> GetAllSum() const {
		std::pair result{ this->sumValue_, this->squaredValues_ };
		return result;
	}

	[[nodiscard]] auto ExpectedValue() const -> T {
		return this->sumValue_ / this->values_.size();
	}

	[[nodiscard]] auto BiasedVarianceEstimate() const -> T {
		return this->sumSquaredValue_ / this->values_.size() -
			std::pow(this->sumValue_, T{ 2.0 }) / std::pow(this->values_.size(), T{ 2.0 });
	}

	[[nodiscard]] auto UnbiasedVarianceEstimate() const -> T {
		return this->sumSquaredValue_ / (this->values_.size() - 1u) -
			std::pow(this->sumValue_, T{ 2.0 }) / (this->values_.size() - 1u) / this->values_.size();
	}

	[[nodiscard]] auto BiasedStandardDeviationEstimate() const -> T {
		return std::sqrt(this->BiasedVarianceEstimate());
	}

	[[nodiscard]] auto UnbiasedStandardDeviationEstimate() const -> T {
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
	T sumValue_;
	T sumSquaredValue_;

	std::vector<T> values_;
	std::vector<T> squaredValues_;

	void AddToAllValues(T value) {
		this->values_.push_back(value);
		this->squaredValues_.push_back(std::pow(value, T{ 2.0 }));
	}

	void UpdateAllSums() {
		this->sumValue_        = this->CalculateSum(this->values_);
		this->sumSquaredValue_ = this->CalculateSum(this->squaredValues_);
	}

	template<class T>
	auto CalculateSum(const std::vector<T>& v) -> T {
		return std::reduce(v.cbegin(), v.cend(), T{}, std::plus());
	}
};
