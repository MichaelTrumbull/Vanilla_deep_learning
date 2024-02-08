MNIST_TRAIN_PATH = "../../MNIST/mnist_train.csv"
MNIST_TEST_PATH = "../../MNIST/mnist_test.csv"

import random
import pandas as pd

print("MNIST_TRAIN_PATH = ../../MNIST/mnist_train.csv")

### MATH FUNCTIONS ###

# np.random.rand()
def rand(dim1,dim2): # offset value included?
    list_rand_floats = [[random.uniform(0,1) - 0.5 for _ in range(dim2)] for _ in range(dim1)]
    return list_rand_floats
# np.maximum()
def maximum(A,compare_val):
    assert isinstance(compare_val, int)
    # 1d 
    if not isinstance(A[0], list):
        #make B the same length as A
        B = []
        for val in A:
            if val > compare_val:
                B.append(val)
            else:
                B.append(compare_val)
        return B
    if isinstance(A[0], list):
        B = []
        for row in A:
            B.append(maximum(row,compare_val))
        return B
    return None

        

# .T
def transpose(A):
    B =[[row[i] for row in A] for i in range(len(A[0]))]
    return B

# np.exp()
def exp(x):
    #print('def exp (x): x = ', x)
    if isinstance(x, int) or isinstance(x, float):
        return 2.71828**x
    
    if isinstance(x,list):
        if isinstance(x[0], list): # 2d list case
            hold = []
            for row in x:
                hold.append( exp(row) )
            return hold
        else: # 1d list case
            hold = []
            #print('def exp (x): x = ', x)
            for i in range(len(x)):
                hold.append(2.71828**x[i])
            return hold
# np.zeros()
def zeros(a,b):
    #x = [[0]*b]*a # dont use this version in python because it uses references so changing one messes with the others
    x = [[0 for _ in range(b)] for _ in range(a)]
    return x

# np.arange()
def arange(n):
    l = [val for val in range(n)]
    return l


# np.argmax()
def argmax(A, axis):
    # 1d 
    if not isinstance(A[0], list):
        largest_val = A[0]
        index = 0
        for i, val in enumerate(A):
            if val > largest_val:
                largest_val = val
                index = i
        return index
    #assume 2d list
    hold_indexes = []
    # 0 index means check row by row
    if axis == 1:
        for row in A:
            hold_indexes.append(argmax(row,0))
        return hold_indexes
    if axis == 0:
        for row in transpose(A):
            hold_indexes.append(argmax(row,0))
        return hold_indexes
    return None


# .size
#def size(A):
#    if isinstance(A[0], list):
#        n, m = shape(A)
#        return int(n * m)
#    return int(shape(A))

# .dot()
def dot(A,B):
    # if A and B are >2d matricies then do matrix multiplication
    if isinstance(A[0], list) and isinstance(B[0], list):
        #print('ran both 2d')
        c = [[0 for _ in range(len(B[0]))] for _ in range(len(A))]
        #print(shape(c))
        for i in range(len(A)):
            for j in range(len(B[0])):
                for k in range(len(B)):

                    c[i][j] += A[i][k] * B[k][j]
        return c
    if not isinstance(A[0], list) and not isinstance(B[0], list):
        #print('ran both 1d')
        res = 0
        for i in range(len(A)):
            res += A[i]*B[i]
        return res
    return 0

# .shape
def shape(A):
    if not isinstance(A,list):
        return 1
    if isinstance(A[0], list):
        return (len(list(A)), len(list(A)[0]))
    if isinstance(A,list):
        return (len(list(A)))
    return None


def div_element_wise(A,n):
    B = [[x/n for x in lst] for lst in A]
    return B
def add_vecs_element_wise(a,b):
    assert len(a) == len(b)
    c = []
    for i in range(a):
        c.append(a[i] + b[i])
    return c
    
# 2d matrix + 1d array 
def add_2dw_with_1d(A,b):
    assert len(A) == len(b) # dims are wrong i think?
    C = []
    for i in range(len(b)):
        hold_row = []
        for val in A[i]:
            # This [0] term is becaus I need to leave it as a 2d array for the numpy version which i'm using while testing
            hold_row.append(val + b[i][0]) 
        C.append(hold_row)
    return C
def sum(A):
    if isinstance(A[0], list):
        hold = 0
        for row in A:
            hold += sum(row)
        return hold
    hold = 0
    for val in A:
        hold += val
    return hold
def subtract_lists(A,B):
    assert len(A) == len(B)
    if isinstance(A[0], list) and isinstance(B[0], list):
        assert len(A[0]) == len(B[0])
        hold2d = []
        for i in range(len(A)):
            hold1d = []
            for j in range(len(A[0])):
                hold1d.append(A[i][j] - B[i][j])
            hold2d.append(hold1d)
        return hold2d
    raise Exception("both must be 2d lists of same size")
def mult_element_wise(val, A):
    assert not isinstance(val,list)
    assert isinstance(A,list)
    if isinstance(A[0], list): #2d list
        hold2d = []
        for row in A:
            hold1d = []
            for a in row:
                hold1d.append(val * a)
            hold2d.append(hold1d)
        return hold2d
    hold = [] # 1d matrix
    for a in A:
        hold.append(val*a)
    return hold

def each_element_is_greater(A, val):
    assert isinstance(A, list)
    assert isinstance(A[0], list)
    hold2d = []
    for row in A:
        hold1d = []
        for a in row:
            hold1d.append(a > val)
        hold2d.append(hold1d)
    return hold2d
            
def mult_lists(A,B):
    assert isinstance(A,list) and isinstance(B,list)
    assert isinstance(A[0], list) and isinstance(B[0], list)
    assert len(A) == len(B)
    assert len(A[0]) == len(B[0])
    hold2d = []
    for i in range(len(A)):
        hold1d = []
        for j in range(len(A[0])):
            hold1d.append( A[i][j] * B[i][j] )
        hold2d.append(hold1d)
    return hold2d

def subtract_val_from_list(A, val):
    if not isinstance(A[0], list):
        hold = []
        for a in A:
            hold.append(a-val)
        return hold
    if isinstance(A[0],list):
        hold = []
        for row in A:
            hold.append(subtract_val_from_list(row,val))
        return hold
def size(A):
    if isinstance(A[0], list):
        hold = 0
        for row in A:
            hold += len(row)
        return hold
    if isinstance(A,list):
        return len(A)
    return 0

def compare_bool_lists(A , B):
    assert not isinstance(A[0], list) # 1d
    assert not isinstance(A[0], list) # 1d
    hold = []
    for a,b in zip(A,B):
        hold.append( a == b )
    return hold




### BEGIN ###




### 2 LAYER FEED FORWARD NETWORK FUNCTIONS

def init_params():
    W1 = rand(10, 784) #np.random.rand(10, 784) - 0.5
    b1 = rand(10, 1) #np.random.rand(10, 1) - 0.5
    W2 = rand(10, 10) #np.random.rand(10, 10) - 0.5
    b2 = rand(10, 1) #np.random.rand(10, 1) - 0.5
    return W1, b1, W2, b2

def ReLU(Z):
    return maximum(Z, 0)#np.maximum(Z, 0)

def softmax(Z):
    A = div_element_wise(exp(Z), sum(exp(Z))) #np.exp(Z) / sum(np.exp(Z))
    return A

def forward_prop(W1, b1, W2, b2, X):
    Z1 = add_2dw_with_1d( dot(W1,X) , b1) #W1.dot(X) + b1
    A1 = ReLU(Z1)
    Z2 = add_2dw_with_1d( dot(W2,A1) , b2) #W2.dot(A1) + b2
    A2 = softmax(Z2)
    # GET THE DIMS OF ALL THESE MATRICIES
    #print('W1: ', shape(W1))
    #print('b1: ', shape(b1))
    #print('W2: ', shape(W2))
    #print('b2: ', shape(b2))
    #print('X: ', shape(X))
    #print('Z1: ', shape(Z1))
    #print('A1: ', shape(A1))
    #print('Z2: ', shape(Z2))
    #print('A2: ', shape(A2))
    #print()
    
    return Z1, A1, Z2, A2

def ReLU_deriv(Z):
    return each_element_is_greater(Z, 0) #return Z > 0

def one_hot(Y):
    one_hot_Y = zeros(size(Y), max(Y) + 1) #np.zeros((Y.size, Y.max() + 1))
    #print('size(Y)',size(Y))
    for a,b in zip(arange(size(Y)), Y): #one_hot_Y[np.arange(size(Y)), Y] = 1 
        one_hot_Y[a][b] = 1
    one_hot_Y = transpose(one_hot_Y)
    #print('shape(one_hot_Y)', shape(one_hot_Y))
    return one_hot_Y

def backward_prop(Z1, A1, Z2, A2, W1, W2, X, Y):
    one_hot_Y = one_hot(Y)
    dZ2 = subtract_lists( A2 , one_hot_Y )
    dW2 = mult_element_wise( 1 / m , dot(dZ2, transpose(A1)) ) #1 / m * dot(dZ2, transpose(A1))
    db2 = 1 / m * sum(dZ2)
    dZ1 = mult_lists( dot(transpose(W2), dZ2) , ReLU_deriv(Z1) )
    dW1 = mult_element_wise( 1 / m , dot(dZ1, transpose(X)) )   #1 / m * dot(dZ1, transpose(X))
    db1 = 1 / m * sum(dZ1)
    #print()
    #print('shape(dZ2)',shape(dZ2))
    #print('shape(dW2)',shape(dW2))
    #print('shape(db2)',shape(db2))
    #print('shape(dZ1)',shape(dZ1))
    #print('shape(dW1)',shape(dW1))
    #print('shape(db1)',shape(db1))

    #print()
    #print('m',m)
    #print()
    
    return dW1, db1, dW2, db2

def update_params(W1, b1, W2, b2, dW1, db1, dW2, db2, alpha):
    W1 = subtract_lists( W1 , mult_element_wise( alpha , dW1) ) #W1 - alpha * dW1
    #print("shape(b1)",shape(b1))
    b1 = subtract_val_from_list(b1, alpha * db1)  #b1 - alpha * db1    
    W2 = subtract_lists( W2 , mult_element_wise( alpha , dW2) ) #W2 - alpha * dW2  
    b2 = subtract_val_from_list(b2, alpha * db2 )  #b2 - alpha * db2    
    return W1, b1, W2, b2

def get_predictions(A2):
    return argmax(A2, 0)

def get_accuracy(predictions, Y):
    return sum(compare_bool_lists(predictions , Y)) / size(Y) #sum(predictions == Y) / Y.size

def gradient_descent(X, Y, alpha, iterations):
    W1, b1, W2, b2 = init_params()
    for i in range(iterations):
        Z1, A1, Z2, A2 = forward_prop(W1, b1, W2, b2, X)
        dW1, db1, dW2, db2 = backward_prop(Z1, A1, Z2, A2, W1, W2, X, Y)
        W1, b1, W2, b2 = update_params(W1, b1, W2, b2, dW1, db1, dW2, db2, alpha)
        if True:#i % 10 == 0:
            print("Iteration: ", i)
            predictions = get_predictions(A2)
            print(get_accuracy(predictions, Y))
    return W1, b1, W2, b2



if __name__ == "__main__":
    #### LOAD DATA / SPLIT ###
    data = pd.read_csv(MNIST_TRAIN_PATH)

    data = data.values.tolist() 
    m, n = shape(data) 

    data_dev = transpose(data[0:1000]) 
    Y_dev = data_dev[0]
    X_dev = data_dev[1:n]
    X_dev = div_element_wise(X_dev, 255.) 

    data_train = transpose(data[1000:m]) 
    Y_train = data_train[0]
    X_train = data_train[1:n]
    X_train = div_element_wise(X_train, 255.) 
    _,m_train = shape(X_train) 

    #print('shape(Y_train)', shape(Y_train))
    #print('shape(X_train)', shape(X_train))
    ### TRAIN THE MODEL ###

    W1, b1, W2, b2 = gradient_descent(X_train, Y_train, 0.10, 3)

