require "autodiff"

function Neuron(nin)
    weights = {}
    for i=1, nin do table.insert(weights,Value(nil)) end
    bias = Value(0)
    return {w=weights, b=bias}
end
function Layer(nin,nout)
    neurons = {}
    for i=1, nout do table.insert(neurons, Neuron(nin)) end
    return neurons
end
function MLP(layer_nodes)---------------------------------------- 
    --nouts is a table of nout for each layer
    layers = {}
    for i=1,(#layer_nodes-1) do
        table.insert(layers, Layer(layer_nodes[i], layer_nodes[i+1]) )
    end
    return layers
end
function printneuron(n)
    print('W')
    for _,v in ipairs(n.w) do
        printvalue(v)
    end
    print('b')
    printvalue(n.b)
    end
function call_neuron(n,input)
    tmp = {}
    for i,val in ipairs(n.w) do
        table.insert(tmp,mul(val, input[i]))
    end
    table.insert(tmp,n.b)
    s = sum(tmp)
    t = tanh(s)
    return t
end

function call_layer(l,input)
    tmp = {}
    for i, n in ipairs(l) do
        table.insert(tmp,call_neuron(n,input))
    end
    return tmp
end

function call_mlp(mlp,x)
    for i,layer in ipairs(mlp) do
        x = call_layer(layer,x)
    end
    return x
end

function mlpParameters(m)
    all_params = {}
    for _,l in ipairs(m) do
        for _,n in ipairs(l) do
            table.insert(all_params, n.b)
            for _,w in ipairs(n.w) do
                table.insert(all_params, w)
            end
        end
    end
    return all_params
end

function train(mlp, xs, ys, lr, epochs)
    params = mlpParameters(mlp)
    for i=1,epochs do
        --predict
        ypred = {}
        for i=1, #ys do
            table.insert(ypred,call_mlp(m, xs[i]))
        end
        --get loss
        mse_table = {}
        for i=1, #ys do
            table.insert( mse_table, pow(sub(ys[i],ypred[1][i]),2) )
        end
        loss = sum(mse_table)
        print(i, loss.data)
        -- zero grad
        for _,v in ipairs(params) do
            v.grad = 0
        end
        -- backward grad pass
        backward(loss)
        for _,v in ipairs(params) do
            v.data = v.data + (lr * v.grad)
        end
    end
end
