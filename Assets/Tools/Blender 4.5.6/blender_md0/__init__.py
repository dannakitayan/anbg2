bl_info = {
    "name": "MD0 Exporter",
    "author": "Dannaki + Maru = Love",
    "version": (1, 0, 0),
    "blender": (4, 5, 6),
    "location": "File > Export > MD0 (.md0)",
    "category": "Import-Export",
}

import bpy
import struct
from bpy_extras.io_utils import ExportHelper


# ---- MD0 constants ----

MD0_MAGIC = b"MDL0"
MD0_VERSION = 1
NAME_MAX = 64


class ExportMD0(bpy.types.Operator, ExportHelper):
    bl_idname = "export_scene.md0"
    bl_label = "Export MD0"
    filename_ext = ".md0"
    
    flip_uv_v: bpy.props.BoolProperty(
        name="Flip UV V",
        description="Flip V coordinate (1.0 - V)",
        default=True,
    )
    
    flip_winding: bpy.props.BoolProperty(
        name="Flip Triangle Winding",
        description="Reverse triangle vertex order",
        default=True,  # ИЗМЕНЕНО: теперь по умолчанию True
    )

    def execute(self, context):
        obj = context.active_object
        if not obj or obj.type != 'MESH':
            self.report({'ERROR'}, "Select a mesh object")
            return {'CANCELLED'}

        scene = context.scene
        depsgraph = context.evaluated_depsgraph_get()
        
        keyframes = set()
        frames_count = 0

        armature = None
        for mod in obj.modifiers:
            if mod.type == 'ARMATURE' and mod.object:
                armature = mod.object
                break

        if armature and armature.animation_data and armature.animation_data.action:
            action = armature.animation_data.action
            for fcurve in action.fcurves:
                for kp in fcurve.keyframe_points:
                    keyframes.add(int(kp.co.x))

        keyframes = sorted(list(keyframes))
        frames_count = len(keyframes)

        # ---- Frame 0 (base mesh) ----

        scene.frame_set(scene.frame_start)
        obj_eval = obj.evaluated_get(depsgraph)
        mesh = obj_eval.to_mesh()
        
        mesh.transform(obj_eval.matrix_world)
        mesh.calc_loop_triangles()

        uv_layer = mesh.uv_layers.active
        color_layer = mesh.color_attributes.active if mesh.color_attributes else None

        # ---- Texture name from material[0] ----

        texture_name = b""
        if obj.material_slots:
            mat = obj.material_slots[0].material
            if mat and mat.use_nodes:
                for node in mat.node_tree.nodes:
                    if node.type == 'TEX_IMAGE' and node.image:
                        texture_name = node.image.name.encode("utf8")
                        break

        texture_name = texture_name[:NAME_MAX]

        # ---- MD0 vertices (ONE vertex = ONE loop) ----

        md0_vertices = []
        
        for loop in mesh.loops:
            v = mesh.vertices[loop.vertex_index]
        
            px, py, pz = v.co
            nx, ny, nz = loop.normal 
        
            if uv_layer:
                uv = uv_layer.data[loop.index].uv
                u = uv.x
                v_uv = (1.0 - uv.y) if self.flip_uv_v else uv.y
            else:
                u, v_uv = 0.0, 0.0
        
            if color_layer:
                col = color_layer.data[loop.index].color
                cr, cg, cb = col[0], col[1], col[2]
            else:
                cr, cg, cb = 1.0, 1.0, 1.0
        
            md0_vertices.append((
                px, py, pz,
                cr, cg, cb,
                u, v_uv,
                nx, ny, nz
            ))
            
        vertices_count = len(md0_vertices)

        # ---- Triangles ----

        triangles = []
        
        for tri in mesh.loop_triangles:
            if self.flip_winding:
                # Reverse winding order (0,1,2) -> (2,1,0)
                triangles.append((
                    tri.loops[2],
                    tri.loops[1],
                    tri.loops[0]
                ))
            else:
                triangles.append((
                    tri.loops[0],
                    tri.loops[1],
                    tri.loops[2]
                ))

        triangles_count = len(triangles)

        # ---- Base positions (per LOOP) ----

        base_positions = []
        for loop in mesh.loops:
            base_positions.append(mesh.vertices[loop.vertex_index].co.copy())

        # ---- Frames (vertex deltas) ----
        
        frames = []
        for frame in keyframes:
            scene.frame_set(frame)
            obj_eval_frame = obj.evaluated_get(depsgraph)
            fmesh = obj_eval_frame.to_mesh()
            
            fmesh.transform(obj_eval_frame.matrix_world)
            
            frame_data = []
            
            # Calculate deltas for each LOOP
            for i, loop in enumerate(fmesh.loops):
                current_pos = fmesh.vertices[loop.vertex_index].co
                delta = current_pos - base_positions[i]
                frame_data.append((delta.x, delta.y, delta.z))
            
            frames.append(frame_data)
            obj_eval_frame.to_mesh_clear()

        # Clean up base mesh
        obj_eval.to_mesh_clear()

        # ---- Write file ----

        with open(self.filepath, "wb") as f:
            # header
            f.write(MD0_MAGIC)
            f.write(struct.pack("<i", MD0_VERSION))
            f.write(struct.pack("<i", triangles_count))
            f.write(struct.pack("<i", vertices_count))
            f.write(struct.pack("<i", frames_count))

            f.write(texture_name)
            f.write(b"\0" * (NAME_MAX - len(texture_name)))

            # vertices
            for v in md0_vertices:
                f.write(struct.pack("<11f", *v))

            # indices
            for tri in triangles:
                f.write(struct.pack("<3i", *tri))

            # frames (vec3_t)
            for frame in frames:
                for d in frame:
                    f.write(struct.pack("<3f", *d))

        self.report({'INFO'}, f"MD0 export complete: {vertices_count} verts, {triangles_count} tris, {frames_count} frames")
        return {'FINISHED'}


# ---- Blender registration ----

def menu_func_export(self, context):
    self.layout.operator(ExportMD0.bl_idname, text="MD0 (.md0)")


def register():
    bpy.utils.register_class(ExportMD0)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)
    bpy.utils.unregister_class(ExportMD0)


if __name__ == "__main__":
    register()