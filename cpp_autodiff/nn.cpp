

Val ReLU(Val & m)
{
    return m.max(0.0);
}

Val softmax(Val & m)
{
    return m.exp() / (m.exp()).sum();
}


Val get_predictions(Val & m)
{
    return m.argmax();
}

float get_accuracy(Val & predictions, Val & Y)
{
    return (predictions == Y).sum() / size_Y;
}

