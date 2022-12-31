Matrix is stoled in row-order

and swaped before copy




Rotation matrix around any axis

\begin{bmatrix}
\cos\theta + u_x^2(1 - \cos\theta)   & u_xu_y(1-\cos\theta) - u_z\sin\theta & u_xu_z(1-\cos\theta) + u_y\sin\theta	& 0\\

u_yu_x(1-\cos\theta) + u_z\sin\theta & \cos\theta + u_y^2(1-\cos\theta)		& u_yu_z(1-\cos\theta)-u_x\sin\theta	& 0\\

u_zu_x(1-\cos\theta) + u_y\sin\theta & u_zu_y(1-\cos\theta) + u_x\sin\theta & \cos\theta+u_z^2(1-\cos\theta)		& 0\\

0									& 0										& 0										& 1\\
\end{bmatrix}


https://en.wikipedia.org/w/index.php?title=Rotation_matrix&action=edit&section=13



TODO

Add posiblity to rotate camera round object in y axis