# 3D Integrated Circuits Recovery Solutions for Faulty TSVs

To ensure the functionality of 3D Integrated Circuits (3D ICs) in the presence of faulty Through-Silicon Vias (TSVs), we propose two approaches: the **Irregular Grid-Based Grouping Algorithm** and the **Regular Grouping-Based Recovery Algorithm**. Both methods involve randomly generating positions for functional TSVs and redundant TSVs within a given die area. This documentation provides an overview of these approaches, illustrated with diagrams showing the distribution and recovery process, making the concept accessible and easy to understand.

## Irregular Grid-Based Grouping Algorithm

In the Irregular Grid-Based Grouping Algorithm, we leverage a grid-based method to group functional and redundant TSVs. This approach adapts dynamically based on the initial distribution and ensures efficient grouping and recovery by adjusting the grid size as needed.

### Key Features:
- **Dynamic Grid Formation**: Partitions the die area into grids based on the number of redundant TSVs (Yline).
- **Adaptive Cell Sizing**: Increases cell size if necessary to meet the required ratio of functional to redundant TSVs.
- **Flexible Grouping**: Allows overlapping cells to ensure all functional TSVs are included in a group.

### Algorithm Details:
1. **Grid Partitioning**: 
   - Determine the partition value \( Yline = \text{number of redundant TSVs} \).
   - If \( Yline > 0.5 \), set \( Yline = \text{ceil}(Yline) \); otherwise, set \( Yline = \text{floor}(Yline) \).
   - Draw \( Yline \) horizontal and vertical lines over the die to form the grid.
2. **Group Formation**:
   - Each cell of the grid acts as a group, including all functional and redundant TSVs within it.
   - If a cell lacks redundant TSVs but contains functional TSVs (or vice versa), or if the ratio of functional to redundant TSVs is lower than the overall ratio, increase the cell size by one unit in each direction.
   - Continue increasing the cell size until the conditions are met. Ignore cells without any TSVs.
   - Allow overlapping of cells to share redundant and functional TSVs between groups.
3. **Final Adjustment**:
   - If any functional TSVs are left ungrouped, assign them to the nearest group containing at least one redundant TSV.

## Regular Grouping-Based Recovery Algorithm

The Regular Grouping-Based Recovery Algorithm uses a systematic Breadth-First Search (BFS) tree formation to organize TSVs. This method ensures a structured and uniform process for identifying and replacing faulty TSVs, leveraging a predictable pattern for grouping and recovery.

### Key Features:
- **Systematic Group Formation**: Utilizes BFS to create structured groups.
- **Uniform Recovery Process**: Ensures a consistent and predictable recovery pattern.
- **Group Ratio**: Ensures each group has the same number of functional TSVs.

### Algorithm Details:
1. **Initialization**: 
   - Count all functional and redundant TSVs.
   - Calculate the group ratio \( \text{group\_ratio} = \frac{\text{Nfunctional\_tsvs}}{\text{Nredundant\_tsvs}} \) and take the ceiling value as the number of functional TSVs each group should have.
2. **Group Formation**:
   - Create different groups for each redundant TSV.
   - For each group, consider a redundant TSV as the root and check the number of functional TSVs within a certain radius (radius = die_area / Nredundant_tsvs).
   - Include these functional TSVs in the group and push them into a queue.
   - For each functional TSV in the queue, check for other functional TSVs within the same radius and add them to the group and queue.
   - Continue until the group size reaches the group_ratio. If the group size is less than the group_ratio and no more nodes are in the queue, increase the radius and repeat the process.
3. **Final Adjustment**:
   - If some functional TSVs are left unallocated, connect them to the nearest functional or redundant TSV already in a group.

## Diagrams

Below, we provide visual depictions of both approaches. Functional TSVs are represented by black dots, and redundant TSVs by red dots. The diagrams illustrate how each approach handles the random placement of TSVs, showing the differences and efficiencies of the two methods.

### Irregular Grid-Based Grouping Algorithm
![Irregular Grid-Based Grouping Algorithm](link_to_diagram_1)

### Regular Grouping-Based Recovery Algorithm
![Regular Grouping-Based Recovery Algorithm](link_to_diagram_2)

By visualizing the distribution and recovery process, these diagrams help in understanding how functional TSVs (black dots) are grouped and linked to redundant TSVs (red dots) in the event of faults, thus illustrating the robustness and effectiveness of our proposed recovery solutions.

---

This README provides a comprehensive overview of our proposed recovery solutions for faulty TSVs in 3D ICs. For further details, refer to the accompanying diagrams and documentation.
