#pragma once
#include <complex>
#include <vector>

template<class T> requires
	std::is_arithmetic_v<T> && (!std::is_same_v<T, bool>) && (!std::is_same_v<T, char>)
class MathSolver {
public:
	explicit MathSolver(T sumValue, T sumSquaredValue, const size_t count) :
		sumValue_(sumValue), sumSquaredValue_(sumSquaredValue), count_(count) {
		
	}

	[[nodiscard]] auto ExpectedValue() const -> T {
		return this->sumValue_ / this->count_;
	}

	[[nodiscard]] auto BiasedVarianceEstimate() const -> T {
		return this->sumSquaredValue_ / this->count_ -
			std::pow(this->sumValue_, T{ 2.0 }) / std::pow(this->count_, T{ 2.0 });
	}

	[[nodiscard]] auto UnbiasedVarianceEstimate() const -> T {
		return this->sumSquaredValue_ / (this->count_ - 1u) -
			std::pow(this->sumValue_, T{ 2.0 }) / (this->count_ - 1u) / this->count_;
	}

	[[nodiscard]] auto BiasedStandardDeviationEstimate() const -> T {
		return std::sqrt(this->BiasedVarianceEstimate());
	}

	[[nodiscard]] auto UnbiasedStandardDeviationEstimate() const -> T {
		return std::sqrt(this->UnbiasedVarianceEstimate());
	}

private:
	T sumValue_;
	T sumSquaredValue_;
	size_t count_;
};
