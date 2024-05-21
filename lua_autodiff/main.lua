require "nn"
require "autodiff"

m = MLP({3,4,4,1})
xs = {
    {2,3,-1},
    {3,-1,0.5},
    {0.5,1,1},
    {1,1,-1}
}
xs = toValues(xs)

ys = {Value(1),Value(-1),Value(-1),Value(1)} -- target

train(m, xs, ys, 0.005, 500)



------ from llama3 on how to parse a table (use for xs and ys split after csv load)
-- Original table
local original_table = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
}

-- Create a new table for the last column
local last_column = {}

-- Loop through each row in the original table
for i, row in ipairs(original_table) do
    -- Remove the last column from the row and add it to the last_column table
    table.insert(last_column, table.remove(row))
end

-- Print the original table
print("Original table:")
for i, row in ipairs(original_table) do
    for j, value in ipairs(row) do
        io.write(value .. " ")
    end
    io.write("\n")
end

-- Print the last column table
print("Last column table:")
for i, value in ipairs(last_column) do
    io.write(value .. "\n")
end