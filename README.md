# Pitch-By-Pitch Baseball Simulator
The goal of this project is to predict the outcome of MLB at-bats using Statcast data by generating probable outcomes of each pitch and stochastically selecting an outcome.

## Inputs
A Python script will be used to make a database of relevant inputs for both pitchers and hitters. The following are the inputs gathered:
1. 
2. 
3. 
4. 
5. 

## Outputs
Below is a list of the possible outputs from the model using the inputs outlined above. Each output will be weighted with a probability and we use a random number generator to select the output for each pitch. This work will be done in a simulation program written in Rust.
1. 
2. 
3. 
4. 
5. 

## Model Description
An at-bat in a vacuum consists of a series of events. The first event, as I define it, is the point where the batter (or in very rare cases the pitcher) selects the arm that they want to bat (or pitch) with. Switch batters will generally choose to take their at-bat from the side opposite batter's box of the pitcher's arm. This is where we start limiting the data. For example, this is the point where we select Tarik Skubal's numbers against right-handed batters in an at-bat versus José Ramírez's numbers againt left-handed pitchers. This leads us to our second event: pitch selection. This part of the formula requires two inputs: the pitcher and the count. Pitchers will generally throw different pitches on different counts. Normally fastballs are thrown early in counts and breaking/offspeed pitches later. Of course this isn't going to be exactly correct. In 2024 Tarik Skubal threw his 4-Seam Fastball 40.3% of the time in an 0-0 count versus right-handed batters, but maybe he knows José Ramírez historically hits the fastball well in an 0-0 count (hypothetically). Using this knowledge he may opt to throw the changeup or some other offering in his repertoire. Thus, the first assumption of our model is that the pitcher will not change his tendencies based on who the batter in the box is. Does this mean our model won't be 100% accurate? Absolutely. Is 100% accuracy possible in a model of this scope? Absolutely not.

The third event is the pitch execution. We received a weighted random output from the last event in the form of a pitch type. 

## Results
To-Do
