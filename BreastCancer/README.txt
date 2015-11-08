-----------------------------------------------------------------------------
What's going on in this directory:

The original dataset from UCI is breast-cancert-wisconsin.data
Modified into our format is bc.data
  bc80 and bc90 are copies of this

Then there are 10 versions of bc80-test-foo       20% of bc.data
Then there are 10 versions of bc80-train-bar      80% of bc.data

Then there are 10 versions of bc90-test-foo       10% of bc.data
Then there are 10 versions of bc90-train-bar      90% of bc.data


Make sure you keep 80's with 80's and 90's with 90's and also keep the testing
and training pairs together according to the final number.

-----------------------------------------------------------------------------
Alterations from breast-cancer-wisconsin to produce bc.data

1. ID numbers removed
     most are 7 digit, but also 8-digit, 6-digit, and 5-digit
2. changed to letters
     a=1, b=2, ... 10=j
3. class attribute moved from last column to first column

CBC
-----------------------------------------------------------------------------


