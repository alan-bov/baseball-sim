# Pitch-By-Pitch Baseball Simulator
The goal of this project is to predict the outcome of MLB at-bats using Statcast data by generating probable outcomes of each pitch and stochastically selecting an outcome.

## Inputs
A Python script will be used to make a database of relevant inputs for both pitchers and hitters. The following are the inputs gathered:
1. I1
2. I2
3. I3
4. I4
5. I5

## Outputs
Below is a list of the possible outputs from the model using the inputs outlined above. Each output will be weighted with a probability and we use a random number generator to select the output for each pitch. This work will be done in a simulation program written in Rust.
1. O1
2. O2
3. O3
4. O4
5. O5

## Model Description
An at-bat in a vacuum consists of a series of events. Here is the list of events that this model uses in order to simulate an at-bat.

### Hand Matchup
The first event is the point where the batter (or in very rare cases the pitcher) selects the arm that they want to bat (or pitch) with. Switch batters will generally choose to take their at-bat from the side opposite batter's box of the pitcher's arm. This is very simple. We take the pitcher's handedness and check to see if the batter bats switch. If they do, we take the opposite arm side of the pitcher. We will start limiting the data used for simulating here. As an example, this is the point where we select Tarik Skubal's numbers against right-handed batters in an at-bat versus José Ramírez's numbers againt left-handed pitchers. Note that the first assumption of our model is that switch batters will not choose to bat from the pitcher's arm side.

### Pitch Selection
This leads us to our second event: pitch selection. This part of the formula requires three inputs: the pitcher's pitch selection filtered by batter handedness, the batter's probability to see certain pitches filtered by pitcher handedness, and the balls and strikes count. Pitchers will generally throw different pitches on different counts to different batters. Normally fastballs are thrown early in counts and breaking/offspeed pitches later. In 2024 Tarik Skubal threw his 4-Seam Fastball 40.3% of the time in an 0-0 count versus right-handed batters, but maybe he knows José Ramírez historically hits the fastball well in an 0-0 count (hypothetically). Using this knowledge he may opt to throw the changeup or some other offering in his repertoire. The pitch selection model takes in the percent of pitch types the batter sees in certain counts in order to correct for situations where pitchers understand that a pitch is more or less effective against certain batters. Thus, we simply model the probabilities as below:

$$
P(p) \propto P_p(p)^{\alpha} \times (1 - P_b(p))^{\beta}
$$

where:

- $P(p)$ is the combined probability of the pitcher throwing pitch $p$.
- $P_p(p)$ is the pitcher’s historical probability of throwing pitch $p$ in the current count and matchup.
- $P_b(p)$ is the batter’s historical probability of seeing pitch $p$ in the current count and matchup.
- $\alpha$ and $\beta$ are tuning parameters that control the influence of the pitcher’s preference and the batter’s expectations, respectively.

After calculating the combined score for each pitch, normalize the probabilities so they sum to 1:

$$
P(p) = \frac{P_p(p)^{\alpha} \times (1 - P_b(p))^{\beta}}{\sum_{p} P_p(p)^{\alpha} \times (1 - P_b(p))^{\beta}}
$$

Please note that we are making an extreme oversimplification of a unique event. There are other factors at play in a real game that we cannot really account for without making this model extremely and unnecessarily complex. Thus, our second assumption of the model is that these are the only inputs required to predict the pitch selection on a given pitch. We are neglecting other factors like the inning, score differential, number of outs, runners on base, pitcher fatigue, weather, ballpark dimensions, defensive positioning, or any other of the seemingly endless factors that may influence the pitch selection. The idea here is to treat each at-bat as if it were in a vacuum.

### Pitch Execution
The third event is the pitch execution. We received a weighted random output from the last event in the form of a pitch type. Now for each pitch we need to generate a probability table where we get a percentage for each possible outcome of the pitch. This may look like:

| Ball            | Called Strike   | Swinging Strike | Foul Ball       | In-Play Single  | In-Play Double  | In-Play Triple  | In-Play HR      | In-Play Out     |
|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
| 0.32            | 0.18            | 0.14            | 0.12            | 0.07            | 0.02            | 0.00            | 0.03            | 0.12            |

We calculate the probabilities, simulate a result randomly, and then recalculate the probabilities again if we did not get a resolution to the at-bat.

## Results
To-Do
