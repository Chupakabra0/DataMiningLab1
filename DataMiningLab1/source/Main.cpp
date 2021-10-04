#include "MainHeader.hpp"

void SetLocale(const unsigned code) {
#ifdef WIN32
	SetConsoleCP(code);
	SetConsoleOutputCP(code);
#endif
}

void PrintDelimiter(std::ostream& out, const char symbol = '-', const size_t count = 40u) {
	out << std::string(count, symbol) << std::endl;
}

template<class FirstIter, class SecondIter>
tabulate::Table SetupTable(const std::string& zeroParagraph, const std::string& firstParagraph,
	const std::string& secondParagraph, FirstIter firstBegin, SecondIter secondBegin, const size_t limit) {
	tabulate::Table result;

	result.add_row({ zeroParagraph, firstParagraph, secondParagraph });
	result.row(0u).format().font_align(tabulate::FontAlign::center);

	for (auto i = 0u; i < limit; ++i) {
		result.add_row({
			std::format("{}", i + 1),
			std::format("{:.4f}", firstBegin[i]),
			std::format("{:.4f}", secondBegin[i])
		});
	}

	return result;
}

int main(const int argc, char* argv[]) {
	SetLocale(1251u);

	if (argc < 2) {
		fmt::print("Добавьте файл с входными данными...\nПример: {} fileName.json\n",
			std::filesystem::path(*argv).filename().string());
		return EXIT_SUCCESS;
	}


	for (auto i = 1; i < argc; ++i) {
		try {
			fmt::print("Файл данных: ");
			fmt::print(fmt::fg(fmt::color::aqua), "{}\n", argv[i]);

			std::fstream file(argv[i], std::ios::in);

			if (!file.is_open()) {
				throw std::runtime_error("Wrong path or file doesn't exist!");
			}

			const std::shared_ptr<IDataParser> parser =
				std::make_shared<JsonDataParser>(file);

			const auto data = parser->GetData();
			const auto sigma = parser->GetSigma();
			const auto [firstAlpha, secondAlpha] = parser->GetAlpha();
			const auto freedomLevel = parser->GetFreedomLevel();

			const auto mathSolver = std::make_shared<MathSolver>(data.begin(), data.end());

			const auto [values, squared] = mathSolver->GetAllInfo();
			const auto dataTable = SetupTable("i", "x_i", "x_i^2",
				values.cbegin(), squared.cbegin(), values.size());

			std::cout << dataTable << std::endl;

			fmt::print("Мат. ожидание: {:.4f}\n", mathSolver->ExpectedValue());
			PrintDelimiter(std::cout);

			fmt::print("Смещённая оценка дисперсии: {:.4f}\n", mathSolver->BiasedVarianceEstimate());
			fmt::print("НЕсмещённая оценка дисперсии: {:.4f}\n", mathSolver->UnbiasedVarianceEstimate());
			PrintDelimiter(std::cout);

			fmt::print("Смещённая оценка среднеквадр. отклонения: {:.4f}\n", mathSolver->BiasedStandardDeviationEstimate());
			fmt::print("НЕсмещённая оценка среднеквадр. отклонения: {:.4f}\n", mathSolver->UnbiasedStandardDeviationEstimate());
			PrintDelimiter(std::cout);

			const auto trustIntervalKnown = mathSolver->TrustIntervalKnown(firstAlpha, sigma);
			fmt::print("Доверительный интервал в случае известной дисперсии при sigma = {:.2f} с доверительной вероятностью {:.2f}%"
				" равен ({:.4f}, {:.4f})\n", sigma, firstAlpha * 100.0, trustIntervalKnown.first, trustIntervalKnown.second);

			const auto trustIntervalUnknown = mathSolver->TrustIntervalUnknown(secondAlpha, freedomLevel);
			fmt::print("Доверительный интервал в случае НЕизвестной дисперсии с доверительной вероятностью {:.2f}%"
				" и степенью свободы {} равен ({:.4f}, {:.4f})\n",
				secondAlpha * 100.0, freedomLevel, trustIntervalUnknown.first, trustIntervalUnknown.second);
		}
		catch (const std::exception& exception) {
			fmt::print(fmt::fg(fmt::color::red), "ОШИБКА!!! {}\n", exception.what());
		}

		PrintDelimiter(std::cout, '=');
	}

	return EXIT_SUCCESS;
}
