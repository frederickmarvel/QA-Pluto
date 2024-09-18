#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <stdexcept>
#include <limits>

class FinanceCalculator {
private:
    const double EPSILON = 1e-9;
    const double protocolFee = 0.1; // Assuming 10% protocol fee

    // Helper function for safe division
    double safeDivide(double a, double b) const {
        if (std::abs(b) < EPSILON) {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    }

    // Helper function to clamp value between 0 and 1
    double clamp(double value) const {
        return std::max(0.0, std::min(1.0, value));
    }

public:
    double calculateLeverageRatio(double collateralUSD, double debtUSD) const {
        return safeDivide(collateralUSD + debtUSD, collateralUSD);
    }

    double calculateDebtUSD(double positionSizeUSD, double collateralUSD) const {
        return positionSizeUSD - collateralUSD;
    }

    double calculateUtilizationRate(double totalDebtUSD, double totalSupplyUSD) const {
        return safeDivide(totalDebtUSD, totalSupplyUSD);
    }

    double calculateLeveragedAPY(double lvAPY, double currentLeverageRatio) const {
        return lvAPY * currentLeverageRatio;
    }

    double calculateBorrowAPYBeforeFee(double utilizationRate, double floorCap, double ceilingCap) const {
        double ur = clamp(utilizationRate);
        if (ur <= 0.8) {
            return (ur / 0.8) * floorCap;
        } else {
            return floorCap + ((ur - 0.8) / 0.2) * (ceilingCap - floorCap);
        }
    }

    double calculateBorrowFee(double borrowAPYBeforeFee, double utilizationRate) const {
        double ur = clamp(utilizationRate);
        return borrowAPYBeforeFee * (protocolFee + (2 * protocolFee * (ur - 0.5)));
    }

    double calculateBorrowAPY(double borrowAPYBeforeFee, double borrowFee) const {
        return borrowAPYBeforeFee + borrowFee;
    }

    double calculateOverallAPY(double leveragedAPY, double collateralUSD, double borrowAPY, double debtUSD) const {
        return safeDivide(leveragedAPY * collateralUSD - borrowAPY * debtUSD, collateralUSD);
    }

    double calculateSupplyAPYBeforeFee(double borrowAPYBeforeFee, double utilizationRate) const {
        return borrowAPYBeforeFee * utilizationRate;
    }

    double calculateSupplyFee(double supplyAPYBeforeFee, double utilizationRate) const {
        double ur = clamp(utilizationRate);
        return supplyAPYBeforeFee * (protocolFee - (2 * protocolFee * (ur - 0.5)));
    }

    double calculateNetSupplyAPY(double supplyAPYBeforeFee, double supplyFee) const {
        return supplyAPYBeforeFee - supplyFee;
    }

    double calculateLiquidationPrice(double currentPriceJLP, double collateral, double liquidationThreshold, double totalPositionValue) const {
        return currentPriceJLP - (currentPriceJLP * safeDivide(collateral * liquidationThreshold, totalPositionValue));
    }

    double calculateHealthFactor(double lvCurrentPrice, double lvTokenAmount, double liquidationThreshold, double debtUSD, double accruedInterest) const {
        return safeDivide(lvCurrentPrice * lvTokenAmount * liquidationThreshold, debtUSD + accruedInterest);
    }
};

void runQATests() {
    FinanceCalculator calculator;
    
    std::cout << "Running QA tests...\n\n";
    std::cout << std::fixed << std::setprecision(6);

    // ... [Rest of the function remains the same] ...
}

int main() {
    try {
        runQATests();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}