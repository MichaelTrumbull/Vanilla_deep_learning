from vanillaPY_nerualnet import * 


#A = [[1,2,3,-1],[3,8,1,0], [9, 1,2,-8]]
#print("A", A)
#axis = 0
#print("axis", axis)
#print("argmax")
#print(argmax(A, axis))

#A = [[1,2,3,-1],[3,8,1,0], [9, 1,2,-8]]
#B = [[3,2,1],[5,4,3],[8,7,6],[1,3,4]]
#print(dot(A,B))

#A = [[1,2,3,-1],[3,8,1,0], [9, 1,2,-8]]
#b = [[1],[2],[3]]
#print(add_2dw_with_1d(A,b))

#A = [[1,2,3,-1],[3,8,1,0], [9, 1,2,-8]]
#print(each_element_is_greater(A, 2))

#A = [[1,2,3,-1],[3,8,1,0], [9, 1,2,-8]]
#print(mult_lists(A,A))


dim1 = 3
dim2 = 4
list_rand_floats = [[random.uniform(0,1) - 0.5 for _ in range(dim2)] for _ in range(dim1)]
print(list_rand_floats)