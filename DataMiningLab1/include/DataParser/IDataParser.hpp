#pragma once
#include <vector>

struct IDataParser {
public:
	virtual ~IDataParser() = default;

	[[nodiscard]] virtual std::vector<double> GetData() const = 0;

	[[nodiscard]] virtual double GetSigma() const = 0;

	[[nodiscard]] virtual unsigned GetFreedomLevel() const = 0;

	[[nodiscard]] virtual std::pair<double, double> GetAlpha() const = 0;
};
