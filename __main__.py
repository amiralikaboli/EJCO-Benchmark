from helpers.checks import check_progs
from helpers.plot import plot
from helpers.wcoj import Algo

if __name__ == "__main__":
    check_progs()
    plot(Algo.GJ)
    plot(Algo.FJ, vectorised=False)
    plot(Algo.FJ, vectorised=True)
