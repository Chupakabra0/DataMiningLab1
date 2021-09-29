#pragma once
#include <complex>
#include <numeric>
#include <vector>

#include "NormalTable.hpp"
#include "StudentsTable.hpp"

class MathSolver {
public:
	//explicit MathSolver(T value) {
	//	this->AddToAllValues(value);
	//	this->UpdateAllSums();
	//}

	//explicit MathSolver(T firstValue, U... restValues) {
	//	this->AddToAllValues(firstValue);
	//	(this->AddToAllValues(static_cast<T>(restValues)), ...);
	//	this->UpdateAllSums();
	//}

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
