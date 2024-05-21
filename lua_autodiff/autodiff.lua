function Value(a)
    if a==nil then
        val = math.random()*2 - 1
    else
        val = a
    end
    return {data = val, grad = 0, _backward = function(g) end} 
end
function printvalue(v)
    print('data', v.data)
    print('grad', v.grad)
    print('_backward', v._backward)
    print('prev:')
    if not (v.prev==nil) then
        for _,val in ipairs(v.prev) do
            print(val)
        end
    end
    print()
end
function add(a, b)
    function func_backward(r)
        a.grad = a.grad + r.grad
        b.grad = b.grad + r.grad
    end
    result = {
        data = a.data + b.data,
        grad = 0,
        _backward = func_backward,
        prev = {a,b}
    }
    return result
end
function sub(a, b)
    function func_backward(r)
        a.grad = a.grad + r.grad
        b.grad = b.grad + r.grad
    end
    result = {
        data = a.data - b.data,
        grad = 0,
        _backward = func_backward,
        prev = {a,b}
    }
    return result
end
function mul(a, b)
    function func_backward(r)
        a.grad = b.data * r.grad
        b.grad = a.data * r.grad
    end
    result = {
        data = a.data * b.data,
        grad = 0,
        _backward = func_backward,
        prev = {a,b}
    }
    return result
end
function pow(a, b)
    -- b must be a number
    function func_backward(r)
        a.grad = a.grad +( (b * ( a.data^(b-1) ))*r.grad )
    end
    result = {
        data = a.data^b,
        grad = 0,
        _backward = func_backward,
        prev = {a}
    }
    return result
end

function relu(a)
    function func_backward(r)
        a.grad = a.grad + ( (r.data > 0) * r.grad ) -- is this logical operation an issue?
    end
    if a.data<0 then
        val = 0
    else
        val = a.data
    end
    result = {
        data = val,
        grad = 0,
        _backward = func_backward,
        prev = {a}
    }
    return result
end
function tanh(a)
    function func_backward(r)
        a.grad = (1 - (r.data ^ 2) ) * r.grad
    end
    result = {
        data = (math.exp(2*a.data) - 1) / (math.exp(2*a.data) + 1),
        grad = 0,
        _backward = func_backward,
        prev = {a}
    }
    return result
end
function sum(tbl) --tbl is a table of Value objects(tables)
    function func_backward(r)
        for _,v in ipairs(r.prev) do
            v.grad = v.grad + r.grad
        end
    end
    incr = 0
    for _,v in ipairs(tbl) do
        incr = incr + v.data
    end
    result = {
        data = incr,
        grad = 0,
        _backward = func_backward,
        prev = tbl
    }
    return result
end

function backward(end_node)
    topo = {} -- build a directed graph
    visited = {} -- needs to behave like a set()
    function build_topo(v)
        if not visited[v] then
            visited[v] = true
            if not (v.prev == nil) then
                for _,child in ipairs(v.prev) do
                    build_topo(child)
                end
            end
            table.insert(topo, v)
        end
    end
    build_topo(end_node)
    -- loop through backwards and run _backward() on each
    end_node.grad = 1
    for i = #topo, 1, -1 do
        topo[i]._backward(topo[i])
    end
end

function toValues(tbl) -- could do this recursively
    tbl_of_values = {}
    if type(tbl[1]) == "table" then
        for i=1, #tbl do
            tmp = {}
            for j=1, #tbl[i] do
                table.insert(tmp, Value(tbl[i][j]))
            end
            table.insert(tbl_of_values, tmp)
        end
    else
        for i=1,#tbl do
            table.insert(tbl_of_values, Value(tbl[i]))
        end
    end
    return tbl_of_values
end
function load_csv(pth)
    -- Load the CSV file
    local file = io.open(pth, "r")
    local values2d = {}
    -- Read the file line by line
    for line in file:lines() do
        -- Split the line into individual values
        local values = {}
        for value in line:gmatch("%s*(%g+)%s*" ) do
            table.insert(values, value)
        end
        table.insert(values2d, values)
    end
    file:close()
    return toValues(values2d)
end