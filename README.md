# Pitch-By-Pitch Baseball Simulator
The goal of this project is to predict the outcome of MLB at-bats using Statcast data by generating probable outcomes of each pitch and stochastically selecting an outcome.

## Model Description
An at-bat in a vacuum consists of a series of events. Here is the list of events that this model uses in order to simulate an at-bat.

### Hand Matchup
The first event is the point where the batter (or in very rare cases the pitcher) selects the arm that they want to bat (or pitch) with. Switch batters will generally choose to take their at-bat from the side opposite batter's box of the pitcher's arm. This is very simple. We take the pitcher's handedness and check to see if the batter bats switch. If they do, we take the opposite arm side of the pitcher. We will start limiting the data used for simulating here. As an example, this is the point where we select Tarik Skubal's data against right-handed batters in an at-bat versus José Ramírez's data againt left-handed pitchers. Note that the first assumption of our model is that switch batters will not choose to bat from the pitcher's arm side. This is a rare occurrance but it happens. In his career, José Ramírez, a switch hitter, has taken two at bats versus right handed pitchers from the right batter's box (both were against position player Josh Rojas).

### Pitch Selection
This leads us to our second event: pitch selection. This part of the formula requires a number of inputs that will be fully defined at the bottom of this section. Handedness of the pitcher versus the batter will effect a pitcher's thought process when he (or the catcher) is selecting the pitch he will throw. As an example, a pitcher will generally be more apt to throwing a slider to an opposite-handed batter or a changeup to a same-handed batter. The count will also effect pitch selection. Most pitchers opt for fastballs in three ball counts in order to avoid a walk. In 2024 Tarik Skubal threw his 4-Seam Fastball 40.3% of the time in an 0-0 count versus right-handed batters, but maybe he knows José Ramírez historically hits the fastball well in an 0-0 count (hypothetically). Using this knowledge he may opt to throw the changeup or some other offering in his repertoire. The pitch selection model takes in the percent of pitch types the batter sees in certain counts in order to correct for situations where pitchers understand that a pitch is more or less effective against certain batters. Here we create our probability table and weight the pitcher's usage to what the batter usually sees. This is the equation:

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

Now we have a probability table where each each item reperesents a pitch in a pitcher's mix, and the percent chance he will throw that pitch in the current count. The next step this model attempts to account for is pitch tunneling. This is when a pitcher uses his previous pitch to trick the eyes of his batter on the current pitch. A well tunneled pitch deceives the batter into thinking the pitch he is getting is the same as the last one he got. For example I will use Tarik Skubal again. Skubal loves to tunnel his changeup off of his fastball low in the zone. The batter swings thinking that he is getting a low fastball and strikes out. Tunneling influences a pitcher's pitch selection and we will attempt to model by first creating a tunneling score for each pitcher based on the release point, speed, spin rate, and spin axis of a pitch.

Once we know that a pitcher's pitches tunnel well of each other and that he uses pitches to tunnel then we can adjust our probability table.

Please note that we are making an extreme oversimplification of a unique event. There are other factors at play in a real game that we cannot really account for without making this model extremely and unnecessarily complex. Thus, our second assumption of the model is that these are the only inputs required to predict the pitch selection on a given pitch. We are neglecting other factors like the inning, score differential, number of outs, runners on base, pitcher fatigue, weather, ballpark dimensions, defensive positioning, or any other of the seemingly endless factors that may influence the pitch selection. The idea here is to treat each at-bat as if it were in a vacuum.

### Pitch Execution
The third event is the pitch execution. We received a weighted random output from the last event in the form of a pitch type. Now for each pitch we need to generate a probability table where we get a percentage for each possible outcome of the pitch. This may look like:

| Ball            | Called Strike   | Swinging Strike | Foul Ball       | In-Play Single  | In-Play Double  | In-Play Triple  | In-Play HR      | In-Play Out     |
|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
| 0.32            | 0.18            | 0.14            | 0.12            | 0.07            | 0.02            | 0.00            | 0.03            | 0.12            |

We calculate the probabilities, simulate a result randomly, and then recalculate the probabilities again if we did not get a resolution to the at-bat.

## Results
To-Do
