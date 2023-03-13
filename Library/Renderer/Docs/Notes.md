Every vulkan object/struct (Vk* E.g VkInstance, VkSurface) is a pointer to a struct;





CommandPool per thread




Using subpasses in a command buffer
When recording to a VkCommandBuffer, we described above that vkCmdBeginRenderPass() and vkCmdEndRenderPass() are used to
wrap the render pass operations. After vkCmdBeginRenderPass() is called, subsequent commands are applied to the first subpass within the render pass.

To move operations to subsequent subpasses, vkCmdNextSubpass() should be called.
Each call of this function moves operations to the next subpass index, in increasing order, until vkCmdEndRenderPass() is called.
Synchronization between access to attachments described in subpass dependencies is handled automatically.