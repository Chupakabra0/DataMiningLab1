#pragma once
#include "IDataParser.hpp"

#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

class JsonDataParser final : public IDataParser {
public:
	static constexpr auto* DATA_STR    = "data";
	static constexpr auto* ALPHA_STR   = "alpha";
	static constexpr auto* SIGMA_STR   = "sigma";
	static constexpr auto* FREEDOM_STR = "freedomLevel";

public:
	explicit JsonDataParser(const std::filesystem::path& filePath) : file_(filePath, std::ios::in) {
		this->ReadFile();
	}

	JsonDataParser(const JsonDataParser&) = delete;

	JsonDataParser(JsonDataParser&&) noexcept = default;

	~JsonDataParser() override = default;

	[[nodiscard]] std::vector<double> GetData() const override {
		return this->json_[JsonDataParser::DATA_STR].get<std::vector<double>>();
	}

	[[nodiscard]] double GetSigma() const override {
		return this->json_[JsonDataParser::SIGMA_STR].get<double>();
	}

	[[nodiscard]] unsigned GetFreedomLevel() const override {
		return this->json_[JsonDataParser::FREEDOM_STR].get<unsigned>();
	}

	[[nodiscard]] std::pair<double, double> GetAlpha() const override {
		return this->json_[JsonDataParser::ALPHA_STR].get<std::pair<double, double>>();
	}

private:
	std::fstream file_;
	nlohmann::json json_{};

	void ReadFile() {
		if (this->file_.is_open()) {
			this->file_ >> this->json_;
		}
		else {
			// TODO: logging
			std::cerr << "FUCK!" << std::endl;
		}
	}
};
