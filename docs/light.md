# The standard local lightning model (Phong)

Consists of four components:
- **_The emissive_** contribution
- **_The specular_** contribution:
 
  Gives the surface a "shiny" appearance. 
- **_The diffuse_** contribution
- **_The ambient_** contribution

## Specular contribution

Here the position and direction of the viewer is important.

The reflection vector:
```
r = 2(n dot l)n - l
```

Specular reflection
```
c_spec = (s_spec ccm m_spec)(cos theta)^m_gls 
```
ccm = component-wise multiplication of colors

**_m_gls_** is the shininess of the material. It controls the wideness the "hotspot". Smaller m_gls produces 
as larger hotspot.

**_m_spec_** controls the hotspot's intensity and diffuse_color. Highly reflective materials will have a higher value.

**_s_spec_** is basically the diffuse_color of the light source.

Clamp c_spec to zero, in case cos theta is negative.

## Diffuse contribution

Diffuse lighting simulates light that is reflected randomly in all directions due to the rough nature of the 
surface material. Thus, the location of the viewer is unimportant.

```
c_diff = (s_diff ccm m_diff)(n dot l)
```

**_s_diff_** is basically the diffuse_color of the light source.
**_m_diff_** is the diffuse_color of the light and usually equal to s_spec.

The dot product should be clamped to zero, to avoid the object being lit from behind.
