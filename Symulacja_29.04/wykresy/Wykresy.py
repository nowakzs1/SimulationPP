import matplotlib.pyplot as plt

def takeValues(filename):
    lst = []
    
    with open(filename, 'r') as file:
        for line in file:
            l = float(line[:-1])
            lst.append(int(l))
    
    return lst


if __name__ == "__main__":
    
    seed = 100
    
    filename = "uniform_seed_{}.txt".format(seed)
    uniform = takeValues(filename)
    
    filename = "exponential_1_seed_{}.txt".format(seed)
    exp_1 = takeValues(filename)
    
    filename = "exponential_2_seed_{}.txt".format(seed)
    exp_2 = takeValues(filename)
    
    filename = "exponential_3_seed_{}.txt".format(seed)
    exp_3 = takeValues(filename)
    
    # plt.hist(uniform, bins=30, color='skyblue', edgecolor='black')
    
    fig, ((ax0, ax1), (ax2, ax3)) = plt.subplots(nrows=2, ncols=2)
    fig.suptitle('seed {}'.format(seed))
    ax0.hist(uniform, bins=30, color='skyblue', edgecolor='black')
    ax0.set_title("uniform")
    
    ax1.hist(exp_1, bins=100,  color='skyblue', edgecolor='black')
    ax1.set_title("exponential_1")
    
    ax2.hist(exp_2, bins=100, color='skyblue', edgecolor='black')
    ax2.set_title("exponential_2")
    
    ax3.hist(exp_3, bins=100, color='skyblue', edgecolor='black')
    ax3.set_title("exponential_3")
    plt.show()
    
    # fig.savefig("foo.pdf", bbox_inches='tight')
    fig.savefig("Wykresy_seed_{}.pdf".format(seed))
        
    
    print()
    